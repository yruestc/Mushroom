/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Mushroom
 *    > Created Time:  2017-05-05 22:48:10
**/

#ifndef _BOUNDED_QUEUE_HPP_
#define _BOUNDED_QUEUE_HPP_

#include "mutex.hpp"
#include "cond.hpp"

namespace Mushroom {

template<typename T>
class BoundedQueue
{
	public:
		BoundedQueue(int capacity);

		~BoundedQueue();

		inline T* Get();

		inline void Push();

		inline T* Pull(int *pos);

		inline void Put(int pos);

		void Clear();

	private:
		bool   clear_;
		int    capacity_;
		T*    *queue_;
		int   *avail_;
		int   *work_;
		int    front_;
		int    avail_back_;
		int    work_back_;
		Mutex  mutex_;
		Cond   ready_;
		Cond   empty_;
};

template<typename T>
BoundedQueue<T>::BoundedQueue(int capacity)
:clear_(false), capacity_(capacity), front_(0), avail_back_(0), work_back_(0)
{
	if (capacity_ < 64)
		capacity_ = 64;
	if (capacity_ > 4096)
		capacity_ = 4096;

	queue_ = new T*[capacity_];
	for (int i = 0; i != capacity_; ++i)
		queue_[i] = new T;

	avail_ = new int[capacity_];
	for (int i = 0; i != capacity_; ++i)
		avail_[i] = i;

	work_ = new int[capacity_];
	for (int i = 0; i != capacity_; ++i)
		work_[i] = -1;
}

template<typename T>
BoundedQueue<T>::~BoundedQueue()
{
	if (!clear_)
		Clear();

	delete [] avail_;
	delete [] work_;

	for (int i = capacity_; i;)
		delete queue_[--i];

	delete [] queue_;
}

template<typename T>
void BoundedQueue<T>::Clear()
{
	mutex_.Lock();
	while (front_ != avail_back_ || front_ != work_back_)
		empty_.Wait(mutex_);

	clear_ = true;
	mutex_.Unlock();

	ready_.Broadcast();
}

template<typename T>
inline T* BoundedQueue<T>::Get()
{
	mutex_.Lock();
	while (avail_[front_] < 0)
		empty_.Wait(mutex_);
	return queue_[avail_[front_]];
}

template<typename T>
inline void BoundedQueue<T>::Push()
{
	work_[front_] = avail_[front_];
	avail_[front_++] = -1;
	if (front_ == capacity_) front_ = 0;
	mutex_.Unlock();
	ready_.Signal();
}

template<typename T>
inline T* BoundedQueue<T>::Pull(int *pos)
{
	mutex_.Lock();
	while (work_[work_back_] < 0 && !clear_)
		ready_.Wait(mutex_);

	if (clear_) {
		mutex_.Unlock();
		return 0;
	}

	*pos = work_[work_back_];
	T *ret = queue_[*pos];
	work_[work_back_++] = -1;
	if (work_back_ == capacity_) work_back_ = 0;
	mutex_.Unlock();
	return ret;
}

template<typename T>
inline void BoundedQueue<T>::Put(int pos)
{
	mutex_.Lock();
	avail_[avail_back_++] = pos;
	if (avail_back_ == capacity_) avail_back_ = 0;
	mutex_.Unlock();
	empty_.Signal();
}

} // namespace Mushroom

#endif /* _BOUNDED_QUEUE_HPP_ */