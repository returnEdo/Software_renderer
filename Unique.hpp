#pragma once



namespace Memory
{


template <typename T>
class Unique
{
	private:
	
	T*	mPointer { nullptr };

	public:

	Unique(void)				{}
	Unique(T* tPointer): mPointer(tPointer)	{}

	~Unique(void)	{ if (mPointer)	{ delete mPointer; } }

		
	T* get(void)		{ return mPointer; }
	void set(T* tPointer)	{ mPointer = tPointer; }

	template <typename U>
	U& get(void)
	{
		return *(static_cast<U*>(mPointer));
	}


	template <typename U>
	void copy(U tOther)
	{
		mPointer = static_cast<T*>(new U(tOther));
	}


	template <class ... Types>
	void allocate(Types ... tArgs)
	{
		mPointer = new T(tArgs ...);
	}


	template <typename U, class ... Types>
	void allocate(Types ... tArgs)
	{
		mPointer = (T*)(new U(tArgs ...));
	}


	template <typename U>
	void moveTo(Unique<U>& tOther)
	{
		tOther.set((U*)mPointer);
		mPointer 		= nullptr;
	}


	T& operator *(void)
	{
		return *mPointer;
	}


	T* operator ->(void)
	{
		return mPointer;
	}
};


}
