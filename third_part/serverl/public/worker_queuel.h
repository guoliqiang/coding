#ifndef WORKERQUEUEL_H_
#define WORKERQUEUEL_H_

#include <stdlib.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include "client_sock.h"
#include "cqlist.h"
#include "mpool.h"
#include <string>
#include <pthread.h>
#include "packet_router.h"
#include <stdint.h>
#include "base/public/logging.h"

#define WORKER_INPUT_BUFFER_SIZE 40960
#define JOB_QUEUE_DEFAULT_SIZE_LIMIT 1000000

typedef void (*JobFunction)(void* pJob, void* pWorker);
typedef bool (*WorkerReservedInitFunction)(void** pReserved);
typedef void (*SpecialJobCB)(void* param);

typedef struct JobL {
	ClientSock* sock;
	JobFunction fn;
	PacketRouter* packetRouter;
	uint64_t sockKeySnapshot;
	SpecialJobCB specialCB;
	void* specialParam;
} jobl_t;

class WorkerQueueL {
 public:
	typedef struct WorkerL {
		pthread_t thread;
		int id;
		bool run;
		WorkerQueueL * workerQueue;
		ProtocalPackege * inputBuffer;
		uint64_t curLogid;
		jobl_t* currentJob;
		void* reserved;
		uint64_t pcnt;
	} workerl_t;

 public:
	WorkerQueueL();
	~WorkerQueueL();

	bool pushJob(const JobL& job);
	bool fetchJob(JobL& job);
	bool init(int workerCount, WorkerReservedInitFunction reservedInitFn,
            void* _server);
	void setJobQueueLimit(uint32_t limit);
	void* getServer();
	void stop();

 public:
	static pthread_key_t m_workerKey;

 private:
	int m_nWorkerCount;
	uint32_t m_nJobQueueSizeLimit;
	pthread_mutex_t m_jobsMutex;
	pthread_cond_t m_jobsCond;
	cqlist<jobl_t> m_jobs;
	void* server;
	workerl_t* m_workers;
};
#endif /* WORKERQUEUE_H_ */
