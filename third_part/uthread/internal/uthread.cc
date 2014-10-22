// uthread - A user mode threading library
// See the copyright notice in uthread.h.

#include <stdlib.h>
#include "third_part/uthread/public/uswap.h"
#include "third_part/uthread/public/uthread.h"
#include "base/public/logging.h"

namespace uthread {

// object function table
typedef struct uthread_type_t uthread_type_t;

// object types
typedef struct uthread_event_t uthread_event_t;
typedef struct uthread_queue_t uthread_queue_t;
typedef struct uthread_thread_t uthread_thread_t;

// queue entry type
typedef struct uthread_entry_t uthread_entry_t;

// scheduler type
typedef struct uthread_sched_t uthread_sched_t;


#define UTHREAD_BASE \
  const uthread_type_t *type; \
  uthread_h next_object; \
  uthread_h prev_object; \
  uthread_entry_t *queue_list; \
  uthread_thread_t *wait_list;

struct uthread_t {
  UTHREAD_BASE
};

struct uthread_type_t {
  void (*close)(uthread_h object);
  int (*state)(uthread_h object);
  void (*destruct)(uthread_h object);
};

struct uthread_event_t {
  UTHREAD_BASE
};

struct uthread_entry_t {
  uthread_entry_t *prev_entry;
  uthread_entry_t *next_entry;
  uthread_entry_t *next_queue;
  uthread_queue_t *queue;
  uthread_h object;
  int state;
};

struct uthread_queue_t {
  UTHREAD_BASE
  uthread_entry_t *entry_list;
  uthread_entry_t *signal_list;
};

struct uthread_thread_t {
  UTHREAD_BASE
  uthread_context_t context;
  uthread_func_t start_func;
  void *start_arg;
  uthread_sched_t *scheduler;
  uthread_thread_t *next_thread;
  uthread_h wait_object;
  int state;
};

struct uthread_sched_t {
  uthread_thread_t *main_thread;
  uthread_thread_t *current_thread;
  uthread_thread_t *dead_thread;
  int thread_count;
  uthread_thread_t *schedule_list;
  uthread_h objects;
};

// private object funtions
static void uthread_object_wait(uthread_h object, uthread_thread_t *thread);
static void uthread_object_unblock(uthread_h object, uthread_thread_t *thread);
static void uthread_object_add_entry(uthread_h object, uthread_entry_t *entry);
static void uthread_object_drop_queue(uthread_h object, uthread_queue_t *queue);
static void uthread_object_broadcast(uthread_h object, int state);

// private event functions
static uthread_event_t *uthread_event_make(void);
static void uthread_event_close(uthread_h object);
static int uthread_event_state(uthread_h object);
static void uthread_event_destruct(uthread_h object);

// private queue functions
static uthread_queue_t *uthread_queue_make(void);
static void uthread_queue_signal(uthread_queue_t *queue,
                                 uthread_entry_t *entry);
static void uthread_queue_insert(uthread_queue_t *queue,
                                 uthread_entry_t *entry);
static void uthread_queue_clear(uthread_queue_t *queue);
static void uthread_queue_close(uthread_h object);
static int uthread_queue_state(uthread_h object);
static void uthread_queue_destruct(uthread_h object);

// private thread functions
static void uthread_thread_start(void *arg);
static uthread_thread_t *uthread_thread_make(uthread_sched_t *scheduler);
static void uthread_thread_close(uthread_h object);
static int uthread_thread_state(uthread_h object);
static void uthread_thread_destruct(uthread_h object);

// private scheduler functions
static uthread_sched_t *uthread_sched_make(void);
static void uthread_sched_destruct(uthread_sched_t *sched);
static void uthread_sched_insert(uthread_sched_t *sched, uthread_h object);
static void uthread_sched_remove(uthread_sched_t *sched, uthread_h object);
static void uthread_sched_schedule(uthread_sched_t *sched,
                                   uthread_thread_t *threads);
static void uthread_sched_yield(uthread_sched_t *sched);
static uthread_thread_t *uthread_sched_thread(uthread_sched_t *sched);
static void uthread_sched_jump(uthread_sched_t *sched);
static void uthread_sched_exit(uthread_sched_t *sched, int destruct);
static void uthread_sched_close(uthread_sched_t *sched,
                                uthread_thread_t *thread);

// object types
static const uthread_type_t uthread_event_table = {
  uthread_event_close,
  uthread_event_state,
  uthread_event_destruct
};

static const uthread_type_t uthread_queue_table = {
  uthread_queue_close,
  uthread_queue_state,
  uthread_queue_destruct
};

static const uthread_type_t uthread_thread_table = {
  uthread_thread_close,
  uthread_thread_state,
  uthread_thread_destruct
};

static const uthread_type_t *uthread_type_event = &uthread_event_table;
static const uthread_type_t *uthread_type_queue = &uthread_queue_table;
static const uthread_type_t *uthread_type_thread = &uthread_thread_table;


// private object funtions
static void uthread_object_wait(uthread_h object, uthread_thread_t *thread) {
  CHECK(thread->wait_object == NULL);
  if (object->wait_list != NULL) {
    thread->next_thread = object->wait_list->next_thread;
    object->wait_list->next_thread = thread;
  }
  thread->wait_object = object;
  thread->state = UTHREAD_NONSIGNALED;
  object->wait_list = thread;
}

static void uthread_object_unblock(uthread_h object, uthread_thread_t *thread) {
  uthread_thread_t *index = object->wait_list;
  CHECK(thread->wait_object == object);
  CHECK(index != NULL);
  while (index->next_thread != thread &&
         index->next_thread != object->wait_list) {
    index = index->next_thread;
  }
  CHECK(index->next_thread == thread);
  if (index == thread) object->wait_list = NULL;
  else {
    index->next_thread = thread->next_thread;
    thread->next_thread = thread;
    if (object->wait_list == thread) object->wait_list = index;
  }
  thread->state = UTHREAD_NONSIGNALED;
  thread->wait_object = NULL;
}

static void uthread_object_add_entry(uthread_h object, uthread_entry_t *entry) {
  entry->object = object;
  entry->state = UTHREAD_NONSIGNALED;
  if (object->queue_list == NULL) entry->next_queue = entry;
  else {
    entry->next_queue = object->queue_list->next_queue;
    object->queue_list->next_queue = entry;
  }
  object->queue_list = entry;
}

static void uthread_object_drop_queue(uthread_h object,
    uthread_queue_t *queue) {
  uthread_entry_t *entry = object->queue_list;
  uthread_entry_t *next;
  CHECK(entry != NULL);
  do {
    next = entry->next_queue;
    if (next->queue != queue) entry = next;
    else {
      entry->next_queue = next->next_queue;
      next->object = NULL;
      next->next_queue = NULL;
    }
  } while (next != object->queue_list);
  object->queue_list = entry->next_queue == NULL ? NULL : entry;
}

static void uthread_object_broadcast(uthread_h object, int state) {
  uthread_thread_t *thread = object->wait_list;
  uthread_entry_t *entry = object->queue_list;
  // unblock threads
  if (thread != NULL) {
    do {
      thread->state = state;
      thread->wait_object = NULL;
      thread = thread->next_thread;
    } while (thread != object->wait_list);
    object->wait_list = NULL;
    uthread_sched_schedule(thread->scheduler, thread);
  }
  // signal queues
  if (entry != NULL) {
    uthread_entry_t *next = entry->next_queue;
    do {
      entry = next;
      next = entry->next_queue;
      entry->next_queue = NULL;
      entry->state = state;
      uthread_queue_signal(entry->queue, entry);
    } while (entry != object->queue_list);
    object->queue_list = NULL;
  }
}

// private event functions
static uthread_event_t *uthread_event_make(void) {
  uthread_event_t *event =
      (uthread_event_t *)calloc(1, sizeof(uthread_event_t));
  CHECK(event != NULL);
  event->type = uthread_type_event;
  return event;
}

static void uthread_event_close(uthread_h object) {
  uthread_event_t *event = (uthread_event_t *)object;
  CHECK(object->type == uthread_type_event);
  uthread_object_broadcast(object, UTHREAD_CLOSED);
  free(event);
}

static int uthread_event_state(uthread_h object) {
  CHECK(object->type == uthread_type_event);
  return UTHREAD_NONSIGNALED;
}

static void uthread_event_destruct(uthread_h object) {
  uthread_event_t *event = (uthread_event_t *)object;
  CHECK(object->type == uthread_type_event);
  free(event);
}

// private queue functions
static uthread_queue_t *uthread_queue_make(void) {
  uthread_queue_t *queue =
      (uthread_queue_t *)calloc(1, sizeof(uthread_queue_t));
  CHECK(queue != NULL);
  queue->type = uthread_type_queue;
  return queue;
}

static void uthread_queue_signal(uthread_queue_t *queue,
    uthread_entry_t *entry) {
  CHECK(entry->queue == queue);
  CHECK(entry->next_queue == NULL);
  if (entry->prev_entry != NULL) {
    entry->prev_entry->next_entry = entry->next_entry;
  } else {
    queue->entry_list = entry->next_entry;
  }
  if (entry->next_entry != NULL) {
    entry->next_entry->prev_entry = entry->prev_entry;
  }
  entry->prev_entry = NULL;
  if (queue->signal_list == NULL) {
    entry->next_entry = entry;
    uthread_object_broadcast((uthread_h)queue, UTHREAD_SIGNALED);
  } else {
    entry->next_entry = queue->signal_list->next_entry;
    queue->signal_list->next_entry = entry;
  }
  queue->signal_list = entry;
}

static void uthread_queue_insert(uthread_queue_t *queue,
    uthread_entry_t *entry) {
  CHECK(entry->queue == NULL);
  CHECK(entry->prev_entry == NULL && entry->next_entry == NULL);
  entry->queue = queue;
  entry->next_entry = queue->entry_list;
  if (entry->next_entry != NULL) {
    entry->next_entry->prev_entry = entry;
  }
  queue->entry_list = entry;
}

static void uthread_queue_clear(uthread_queue_t *queue) {
  uthread_entry_t *entry;
  for (entry = queue->entry_list; entry != NULL; ) {
    uthread_entry_t *next = entry->next_entry;
    if (entry->next_queue != NULL) {
      uthread_object_drop_queue(entry->object, queue);
    }
    entry->queue = NULL;
    free(entry);
    entry = next;
  }
  queue->entry_list = NULL;
  if (entry = queue->signal_list, entry != NULL) {
    do {
      uthread_entry_t *next = entry->next_entry;
      entry->queue = NULL;
      free(entry);
      entry = next;
    } while (entry != queue->signal_list);
    queue->signal_list = NULL;
  }
}

static void uthread_queue_close(uthread_h object) {
  uthread_queue_t *queue = (uthread_queue_t *)object;
  CHECK(object->type == uthread_type_queue);
  uthread_queue_clear(queue);
  uthread_object_broadcast(object, UTHREAD_CLOSED);
  free(queue);
}

static int uthread_queue_state(uthread_h object) {
  uthread_queue_t *queue = (uthread_queue_t *)object;
  CHECK(object->type == uthread_type_queue);
  return queue->signal_list == NULL ? UTHREAD_NONSIGNALED : UTHREAD_SIGNALED;
}

static void uthread_queue_destruct(uthread_h object) {
  uthread_queue_t *queue = (uthread_queue_t *)object;
  CHECK(object->type == uthread_type_queue);
  uthread_queue_clear(queue);
  free(queue);
}

// private thread functions
static void uthread_thread_start(void *arg) {
  uthread_sched_t *sched = (uthread_sched_t *)arg;
  uthread_thread_t *thread = sched->current_thread;
  CHECK(thread->scheduler == sched);
  CHECK(thread != sched->main_thread);
  if (sched->dead_thread != NULL) {
    uthread_thread_destruct((uthread_h)sched->dead_thread);
    sched->dead_thread = NULL;
  }
  (*thread->start_func)(thread->start_arg);
  CHECK(thread->wait_object == NULL);
  // indicate that thread is signaled
  thread->state = -1;
  uthread_object_broadcast((uthread_h)thread, UTHREAD_SIGNALED);
  // exit, but do not destruct this thread
  uthread_sched_exit(sched, 0);
}

static uthread_thread_t *uthread_thread_make(uthread_sched_t *scheduler) {
  uthread_thread_t *thread =
      (uthread_thread_t *)calloc(1, sizeof(uthread_thread_t));
  CHECK(thread != NULL);
  thread->type = uthread_type_thread;
  thread->scheduler = scheduler;
  thread->state = UTHREAD_NONSIGNALED;
  thread->next_thread = thread;
  return thread;
}

static void uthread_thread_close(uthread_h object) {
  uthread_thread_t *thread = (uthread_thread_t *)object;
  uthread_sched_t *sched;
  CHECK(object->type == uthread_type_thread);
  sched = thread->scheduler;
  // main thread cannot be closed through its handle
  CHECK(thread != sched->main_thread);
  if (thread->wait_object != NULL) {
    uthread_object_unblock(thread->wait_object, thread);
  }
  uthread_object_broadcast(object, UTHREAD_CLOSED);

  if (thread == sched->current_thread) uthread_sched_exit(sched, 1);
  else uthread_sched_close(sched, thread);
}

static int uthread_thread_state(uthread_h object) {
  uthread_thread_t *thread = (uthread_thread_t *)object;
  CHECK(object->type == uthread_type_thread);
  return thread->state == -1 ? UTHREAD_SIGNALED : UTHREAD_NONSIGNALED;
}

static void uthread_thread_destruct(uthread_h object) {
  uthread_thread_t *thread = (uthread_thread_t *)object;
  CHECK(object->type == uthread_type_thread);
  uthread_context_close(&thread->context);
  free(thread);
}

// private scheduler functions
static uthread_sched_t *uthread_sched_make(void) {
  uthread_thread_t *main_thread;
  uthread_sched_t *sched = (uthread_sched_t *)malloc(sizeof(uthread_sched_t));
  CHECK(sched != NULL);
  main_thread = uthread_thread_make(sched);
  sched->main_thread = main_thread;
  sched->current_thread = main_thread;
  sched->dead_thread = NULL;
  sched->thread_count = 0;
  sched->schedule_list = NULL;
  sched->objects = (uthread_h)main_thread;
  return sched;
}

static void uthread_sched_destruct(uthread_sched_t *sched) {
  uthread_h object;
  if (sched->dead_thread != NULL) {
    uthread_thread_destruct((uthread_h)sched->dead_thread);
  }
  for (object = sched->objects; object != NULL; ) {
    uthread_h next = object->next_object;
    (*object->type->destruct)(object);
    object = next;
  }
  free(sched);
}

static void uthread_sched_insert(uthread_sched_t *sched, uthread_h object) {
  // main thread is never removed
  CHECK(sched->objects != NULL);
  // object may not be inserted before
  CHECK(object->prev_object == NULL && object->next_object == NULL);
  object->next_object = sched->objects;
  object->next_object->prev_object = object;
  sched->objects = object;
  if (object->type == uthread_type_thread) ++sched->thread_count;
}

static void uthread_sched_remove(uthread_sched_t *sched, uthread_h object) {
  // main thread may not be removed
  CHECK(object != (uthread_h)sched->main_thread);
  if (object->prev_object != NULL) {
    object->prev_object->next_object = object->next_object;
  } else {
    sched->objects = object->next_object;
    CHECK(sched->objects != NULL);
  }
  // main thread is always last object
  CHECK(object->next_object != NULL);
  object->next_object->prev_object = object->prev_object;
  object->prev_object = NULL;
  object->next_object = NULL;
  // if this is a non-signaled thread then lower the thread count
  if (object->type == uthread_type_thread &&
      uthread_thread_state(object) != UTHREAD_SIGNALED) {
    CHECK(sched->thread_count > 0);
    --sched->thread_count;
  }
}

static void uthread_sched_schedule(uthread_sched_t *sched,
    uthread_thread_t *threads) {
  if (sched->schedule_list != NULL) {
    uthread_thread_t *next = threads->next_thread;
    threads->next_thread = sched->schedule_list->next_thread;
    sched->schedule_list->next_thread = next;
  }
  sched->schedule_list = threads;
}

static void uthread_sched_yield(uthread_sched_t *sched) {
  if (sched->schedule_list != NULL) {
    uthread_thread_t *current = sched->current_thread;
    uthread_thread_t *next = sched->schedule_list->next_thread;
    if (next->next_thread != next) {
      current->next_thread = next->next_thread;
      sched->schedule_list->next_thread = current;
      next->next_thread = next;
    }
    sched->schedule_list = current;
    sched->current_thread = next;
    uthread_context_swap(&current->context, &next->context);
    if (sched->dead_thread != NULL) {
      uthread_thread_destruct((uthread_h)sched->dead_thread);
      sched->dead_thread = NULL;
    }
  }
}

static uthread_thread_t *uthread_sched_thread(uthread_sched_t *sched) {
  uthread_thread_t *next = NULL;
  if (sched->schedule_list != NULL) {
    next = sched->schedule_list->next_thread;
    if (next->next_thread == next) sched->schedule_list = NULL;
    else {
      sched->schedule_list->next_thread = next->next_thread;
      next->next_thread = next;
    }
  }
  return next;
}

static void uthread_sched_jump(uthread_sched_t *sched) {
  uthread_thread_t *next;
  uthread_thread_t *current = sched->current_thread;
  uthread_thread_t *mainthd = sched->main_thread;
  // only the main thread can suspend without waiting
  CHECK(current == mainthd || current->wait_object != NULL);
  next = uthread_sched_thread(sched);
  // continue in main thread if all others are suspended
  if (next == NULL) {
    // unblock main thread if necessary
    if (mainthd->wait_object != NULL) {
      uthread_object_unblock(mainthd->wait_object, mainthd);
    }
    if (current != mainthd) next = mainthd;
  }
  if (next != NULL) {
    sched->current_thread = next;
    uthread_context_swap(&current->context, &next->context);
    if (sched->dead_thread != NULL) {
      uthread_thread_destruct((uthread_h)sched->dead_thread);
      sched->dead_thread = NULL;
    }
  }
}

static void uthread_sched_exit(uthread_sched_t *sched, int destruct) {
  uthread_thread_t *next;
  uthread_thread_t *current = sched->current_thread;
  // thread must have been explicitly created
  CHECK(current != sched->main_thread);
  // thread cannot be blocked
  CHECK(current->wait_object == NULL);
  // no other threads can be waiting for this thread
  CHECK(current->wait_list == NULL);
  // thread is not a member of any queue
  CHECK(current->queue_list == NULL);
  if (destruct) sched->dead_thread = current;
  else {
    // thread must be signaled
    CHECK(current->state == -1);
    // lower thread count
    CHECK(sched->thread_count > 0);
    --sched->thread_count;
  }
  next = uthread_sched_thread(sched);
  if (next == NULL) {
    // jump back to blocked main thread
    next = sched->main_thread;
    // unblock main thread if necessary
    if (next->wait_object != NULL) {
      uthread_object_unblock(next->wait_object, next);
    }
  }
  sched->current_thread = next;
  uthread_context_jump(&next->context);
}

static void uthread_sched_close(uthread_sched_t *sched,
    uthread_thread_t *thread) {
  // thread must have been explicitly created
  CHECK(thread != sched->main_thread);
  // thread cannot be the current thread
  CHECK(thread != sched->current_thread);
  // thread cannot be blocked
  CHECK(thread->wait_object == NULL);
  // no other threads can be waiting for this thread
  CHECK(thread->wait_list == NULL);
  // thread is not a member of any queue
  CHECK(thread->queue_list == NULL);
  // remove thread from schedule list
  if (sched->schedule_list != NULL) {
    uthread_thread_t *index = sched->schedule_list;
    // locate thread in schedule list
    while (index->next_thread != thread &&
           index->next_thread != sched->schedule_list) {
      index = index->next_thread;
    }
    // remove if found
    if (index->next_thread == thread) {
      if (index == thread) sched->schedule_list = NULL;
      else {
        index->next_thread = thread->next_thread;
        thread->next_thread = thread;
        if (sched->schedule_list == thread) sched->schedule_list = index;
      }
    }
  }
  uthread_thread_destruct((uthread_h)thread);
}

// API functions
void uthread_init(void) {
  uthread_sched_t *sched = uthread_sched_make();
  CHECK(sched != NULL);
  uthread_context_init(&sched->main_thread->context, sched);
}

int uthread_wait(uthread_h object) {
  uthread_sched_t *sched = (uthread_sched_t *)uthread_context_data();
  uthread_thread_t *thread = sched->current_thread;
  if ((*object->type->state)(object) == UTHREAD_NONSIGNALED) {
    uthread_object_wait(object, thread);
    uthread_sched_jump(sched);
  } else {
    thread->state = UTHREAD_SIGNALED;
    uthread_sched_yield(sched);
  }
  return thread->state;
}

void uthread_close(uthread_h object) {
  uthread_sched_t *sched = (uthread_sched_t *)uthread_context_data();
  uthread_sched_remove(sched, object);
  (*object->type->close)(object);
}

int uthread_run(void) {
  uthread_sched_t *sched = (uthread_sched_t *)uthread_context_data();
  // run is only allowed in the main thread
  CHECK(sched->current_thread == sched->main_thread);
  // jump to other thread
  uthread_sched_jump(sched);
  return sched->thread_count == 0 ? UTHREAD_SIGNALED : UTHREAD_NONSIGNALED;
}

uthread_h uthread_create(int stack_size,
    uthread_func_t start_func, void *args) {
  uthread_sched_t * sched = (uthread_sched_t *)uthread_context_data();
  uthread_thread_t * thread = uthread_thread_make(sched);
  thread->start_func = start_func;
  thread->start_arg = args;
  uthread_context_setup(&thread->context, stack_size,
      uthread_thread_start, sched);
  uthread_sched_insert(sched, (uthread_h)thread);
  uthread_sched_schedule(sched, thread);
  return (uthread_h)thread;
}

void uthread_exit(void) {
  uthread_sched_t *sched = (uthread_sched_t *)uthread_context_data();
  uthread_thread_t *current = sched->current_thread;
  if (current != sched->main_thread) {
    uthread_sched_remove(sched, (uthread_h)current);
    uthread_thread_close((uthread_h)current);
  } else {
    uthread_context_exit();
    uthread_sched_destruct(sched);
  }
}

uthread_h uthread_self(void) {
  uthread_sched_t *sched = (uthread_sched_t *)uthread_context_data();
  return sched->current_thread == sched->main_thread ?
      NULL : (uthread_h)sched->current_thread;
}

void uthread_yield(void) {
  uthread_sched_t *sched = (uthread_sched_t *)uthread_context_data();
  uthread_sched_yield(sched);
}

uthread_h uthread_event_create(void) {
  uthread_sched_t *sched = (uthread_sched_t *)uthread_context_data();
  uthread_h event = (uthread_h)uthread_event_make();
  uthread_sched_insert(sched, event);
  return event;
}

void uthread_event_broadcast(uthread_h event) {
  CHECK(event->type == uthread_type_event);
  uthread_object_broadcast(event, UTHREAD_SIGNALED);
}

uthread_h uthread_queue_create(void) {
  uthread_sched_t *sched = (uthread_sched_t *)uthread_context_data();
  uthread_h queue = (uthread_h)uthread_queue_make();
  uthread_sched_insert(sched, queue);
  return queue;
}

void uthread_queue_add(uthread_h queue, uthread_h object) {
  uthread_queue_t *q = (uthread_queue_t *)queue;
  uthread_entry_t *entry;
  CHECK(queue->type == uthread_type_queue);
  entry = (uthread_entry_t *)calloc(1, sizeof(uthread_entry_t));
  uthread_queue_insert(q, entry);
  uthread_object_add_entry(object, entry);
}

int uthread_queue_get(uthread_h queue, uthread_h *object, int *state) {
  uthread_queue_t *q = (uthread_queue_t *)queue;
  uthread_entry_t *entry;
  CHECK(queue->type == uthread_type_queue);
  if (q->signal_list == NULL) return 0;
  entry = q->signal_list->next_entry;
  if (entry->next_entry == entry) {
    q->signal_list = NULL;
  } else {
    q->signal_list->next_entry = entry->next_entry;
  }
  *object = entry->object;
  *state = entry->state;
  entry->next_entry = NULL;
  entry->object = NULL;
  free(entry);
  return 1;
}

void uthread_queue_reset(uthread_h queue) {
  uthread_queue_t *q = reinterpret_cast<uthread_queue_t *>(queue);
  CHECK(queue->type == uthread_type_queue);
  uthread_queue_clear(q);
}

}  // namespace uthread
