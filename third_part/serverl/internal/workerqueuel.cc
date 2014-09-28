#include "../public/workerqueuel.h"
#include "sys/types.h"
#include "../public/serverl.h"

pthread_key_t WorkerQueueL::m_workerKey;

static void* worker_fn(void* arg) {
	WorkerQueueL::workerl_t* self = (WorkerQueueL::workerl_t*)arg;
	jobl_t job;
	bool ok = false;
	self->currentJob = NULL;
	pthread_setspecific(WorkerQueueL::m_workerKey, self);
	LOG(INFO) << "worker-" << self->id << " begin work...";

	while (self->run) {
		self->curLogid = 0;
		ok = self->workerQueue->fetchJob(job);
		if (ok) {
			self->currentJob = &job;
			(job.fn)(&job, self);
			self->currentJob = NULL;
		} else LOG(WARNING) << "worker-" << self->id <<" fetch job failed, continue";
	}
	LOG(INFO) << "worker-" << self->id << " end work";
	return NULL;
}

WorkerQueueL::WorkerQueueL() {
	this->m_nWorkerCount = 0;
	this->m_workers = NULL;
	this->m_nJobQueueSizeLimit = JOB_QUEUE_DEFAULT_SIZE_LIMIT;
	m_jobs.reset(this->m_nJobQueueSizeLimit+10);
}

WorkerQueueL::~WorkerQueueL() {
	pthread_mutex_destroy(&m_jobsMutex);
	pthread_cond_destroy(&m_jobsCond);
	delete []m_workers;
}

void WorkerQueueL::setJobQueueLimit(uint32_t limit) {
	if (limit < 16) {
		LOG(WARNING) << "WorkerQueueL::setJobQueueLimit limit too small, ignore is "
              << limit;
	} else {
		this->m_nJobQueueSizeLimit = limit;
		m_jobs.reset(this->m_nJobQueueSizeLimit+10);
	}
}

bool WorkerQueueL::init(int workerCount,
                        WorkerReservedInitFunction reservedInitFn,
                        void* _server) {
	static bool workeyKeyCreated = false;
	int r;
	this->server = _server;
	r = pthread_mutex_init(&m_jobsMutex,NULL);
	if (r) return false;

	r = pthread_cond_init(&m_jobsCond,NULL);
	if (r) return false;

	if (!workeyKeyCreated) {
		r = pthread_key_create(&m_workerKey, NULL);
		if (r) return false;
		workeyKeyCreated = true;
	}

	this->m_workers = new workerl_t[workerCount];
	if (NULL == this->m_workers) return false;

	this->m_nWorkerCount = workerCount;

	for (int i = 0 ; i < this->m_nWorkerCount ; i ++) {
		this->m_workers[i].inputBuffer = new ProtocalPackege();
		if (this->m_workers[i].inputBuffer==NULL) return false;
		if (this->m_workers[i].inputBuffer->init(WORKER_INPUT_BUFFER_SIZE)==false) {
      return false;
    }

		if (reservedInitFn) {
			if (!reservedInitFn(&(this->m_workers[i].reserved)) ) return false;
		} else {
			m_workers[i].reserved = NULL;
		}

		this->m_workers[i].id = i;
		this->m_workers[i].run = true;
		this->m_workers[i].workerQueue = this;
		this->m_workers[i].pcnt = 0;

		r = pthread_create(&(this->m_workers[i].thread), NULL, worker_fn,
                       (void*)(&(this->m_workers[i])));
		if (r) return false;
	}
	return true;
}

void WorkerQueueL::stop() {
	int i;
	if (NULL != this->m_workers) {
		for (i = 0 ; i < this->m_nWorkerCount ; i ++) {
			this->m_workers[i].run = false;
		}
		for (i = 0 ; i < this->m_nWorkerCount ; i ++) {
			pthread_join(this->m_workers[i].thread, NULL);
		}
	}
}

bool WorkerQueueL::pushJob(const JobL& job) {
	bool ret = false;
	int r = pthread_mutex_lock(&m_jobsMutex);
	if (r) {
		pthread_mutex_unlock(&m_jobsMutex);
		return false;
	}

	if (m_jobs.size() < this->m_nJobQueueSizeLimit) {
		m_jobs.push(job);
		ret = true;
	} else {
		ret = false;
		LOG(WARNING) << "WorkerQueueL::pushJob jobQueueLimit="
              << this->m_nJobQueueSizeLimit << " exceeded, dropping job";
	}
	pthread_cond_signal(&m_jobsCond);
	pthread_mutex_unlock(&m_jobsMutex);
	LOG(INFO) << "WorkerQueueL::pushJob ok: fn=" << job.fn << " router="
             << job.packetRouter << "sock-snapshot="
             << job.sockKeySnapshot << "tspecial-cb="
             << job.specialCB;
	return ret;
}

bool WorkerQueueL::fetchJob(JobL& job) {
	bool ret = false;
	int r = pthread_mutex_lock(&m_jobsMutex);
	if (r) {
	  LOG(WARNING) << "WorkerQueueL::fetchJob lock jobsMutex failed";
		pthread_mutex_unlock(&m_jobsMutex);
		return false;
	}

	while (m_jobs.empty()) {
		pthread_cond_wait(&m_jobsCond, &m_jobsMutex);
	}
	job = m_jobs.front();
	m_jobs.pop();
	ret = true;
	LOG(INFO) << "WorkerQueueL::pushJob ok: fn=" << job.fn << " router="
             << job.packetRouter << "sock-snapshot="
             << job.sockKeySnapshot << "tspecial-cb="
             << job.specialCB;
	pthread_mutex_unlock(&m_jobsMutex);
	return ret;
}

void* WorkerQueueL::getServer() {
	return this->server;
}
