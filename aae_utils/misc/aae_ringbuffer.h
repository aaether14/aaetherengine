#ifndef AAE_RINGBUFFER_H
#define AAE_RINGBUFFER_H
#include "aae_memory.h"


namespace aae
{


template <typename T>
class ringbuffer
{
	T* m_buffer;
	i32 m_head, m_tail;
	aae_size_t m_size;
	AAEAllocatorInterface* m_allocator;
	public:
	ringbuffer(const aae_size_t &size, AAEAllocatorInterface* allocator) : 
		m_buffer(nullptr),
		m_head(0), 
		m_tail(0),
		m_size(size + 1), /** a size of n + 1 is a capacity of n **/
		m_allocator(allocator)
	{
		if (!m_allocator) throw("null allocator reference in ring buffer!");
		if (m_size)
		{
			m_buffer = AAE_NEW_ARRAY(T, m_size, m_allocator);
			if (!m_buffer) throw("failed allocation in ring buffer!");
		}
	}
	virtual ~ringbuffer()
	{
		if (m_buffer && m_allocator) AAE_DELETE_ARRAY(m_buffer, m_allocator);
	}
	inline const aae_size_t& get_size() const { return m_size; }
	inline bool empty() const { return m_head == m_tail; }
	inline bool full() const { return (m_head + 1) % m_size == m_tail; }
	virtual inline bool push(const T& c_data)
	{
		i32 m_next = (m_head + 1) % m_size;
		if (m_next == m_tail) { if (overflow()) return push(c_data); return false; } /** handle the overflow and try to push the data again **/
		m_buffer[m_head] = c_data;
		m_head = m_next;
	}
	virtual inline bool pop(T& c_data)
	{
		/** the return value is garbage, this is why you should check if buffer is empty before popping **/
		if (empty()) { if(underflow()) return pop(c_data); return false; } /** handle the overflow and try to pop again **/
		c_data = m_buffer[m_tail];
		m_tail = (m_tail + 1) % m_size;
		return true;
	}
	protected:
	inline T* const get_buffer() const { return m_buffer; }
	inline const i32& get_tail() const { return m_tail; }
	inline const i32& get_head() const { return m_head; }
	inline void set_tail(const i32& tail) { m_tail = tail % m_size; }
	inline void set_head(const i32& head) { m_head = head % m_size; }
	virtual bool overflow() { return false; }
	virtual bool underflow() { return false; }
};


}
#endif