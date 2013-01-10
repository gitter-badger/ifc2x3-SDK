// IFC SDK : IFC2X3 C++ Early Classes
// Copyright (C) 2009 CSTB
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full license is in Licence.txt file included with this
// distribution or is available at :
//     http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

#ifndef Step_Referenced_h
#define Step_Referenced_h

#include <Step/StepExport.h>
#include <Step/Config.h>
#include <Step/ClassType.h>

#include <vector>

#ifdef STEP_THREAD_SAFE
#  include <osg/Referenced>
#  include <osg/Observer>
#endif

namespace Step {

    class Observer;

#ifdef STEP_THREAD_SAFE
    class STEP_EXPORT Referenced : public osg::Referenced
    {
        ClassType_definitions();

    public:

        Referenced(){}

        explicit Referenced(bool threadSafeRefUnref)  : osg::Referenced(threadSafeRefUnref) {}

        Referenced(const Referenced&ref) : osg::Referenced(ref) {}
    };


    /** Smart pointer for handling referenced counted objects.*/
    template<class T>
    class RefPtr
    {
    public:
        typedef T element_type;

        RefPtr() : _ptr(0) {}
        RefPtr(Referenced* ptr) : _ptr(ptr) { if (_ptr) _ptr->ref(); }
        RefPtr(const RefPtr& rp) : _ptr(rp._ptr) { if (_ptr) _ptr->ref(); }
        template<class Other> RefPtr(const RefPtr<Other>& rp) : _ptr(rp._ptr) { if (_ptr) _ptr->ref(); }

        ~RefPtr() { if (_ptr) _ptr->unref();  _ptr = 0; }

        RefPtr& operator = (const RefPtr& rp)
        {
            assign(rp);
            return *this;
        }

        template<class Other> RefPtr& operator = (const RefPtr<Other>& rp)
        {
            assign(rp);
            return *this;
        }

        inline RefPtr& operator = (T* ptr)
        {
            if (_ptr==ptr) return *this;
            osg::Referenced* tmp_ptr = _ptr;
            _ptr = ptr;
            if (_ptr) _ptr->ref();
            // unref second to prevent any deletion of any object which might
            // be referenced by the other object. i.e rp is child of the
            // original _ptr.
            if (tmp_ptr) tmp_ptr->unref();
            return *this;
        }

#ifdef OSG_USE_REF_PTR_IMPLICIT_OUTPUT_CONVERSION
        // implicit output conversion
        operator T*() const { return reinterpret_cast<T*>(_ptr); }
#else
        // comparison operators for RefPtr.
        bool operator == (const RefPtr& rp) const { return (_ptr==rp._ptr); }
        bool operator == (const T* ptr) const { return (_ptr==ptr); }
        friend bool operator == (const T* ptr, const RefPtr& rp) { return (ptr==rp._ptr); }

        bool operator != (const RefPtr& rp) const { return (_ptr!=rp._ptr); }
        bool operator != (const T* ptr) const { return (_ptr!=ptr); }
        friend bool operator != (const T* ptr, const RefPtr& rp) { return (ptr!=rp._ptr); }

        bool operator < (const RefPtr& rp) const { return (_ptr<rp._ptr); }


        // follows is an implmentation of the "safe bool idiom", details can be found at:
        //   http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Safe_bool
        //   http://lists.boost.org/Archives/boost/2003/09/52856.php

    private:
        typedef T* RefPtr::*unspecified_bool_type;

    public:
        // safe bool conversion
        operator unspecified_bool_type() const { return valid()? &RefPtr::_ptr : 0; }
#endif

        T& operator*() const { return *static_cast<T*>(_ptr); }
        T* operator->() const { return static_cast<T*>(_ptr); }
        T* get() const { return static_cast<T*>(_ptr); }

        bool operator!() const   { return _ptr==0; } // not required
        bool valid() const       { return _ptr!=0; }

        T* release() {
            T* tmp=static_cast<T*>(_ptr);
            if (_ptr)
                _ptr->unref_nodelete();
            _ptr=0;
            return tmp;
        }

        void swap(RefPtr& rp) { T* tmp=_ptr; _ptr=rp._ptr; rp._ptr=tmp; }

    private:

        template<class Other> void assign(const RefPtr<Other>& rp)
        {
            if (_ptr==rp._ptr) return;
            osg::Referenced* tmp_ptr = _ptr;
            _ptr = rp._ptr;
            if (_ptr) _ptr->ref();
            // unref second to prevent any deletion of any object which might
            // be referenced by the other object. i.e rp is child of the
            // original _ptr.
            if (tmp_ptr) tmp_ptr->unref();
        }

        template<class Other> friend class RefPtr;

        osg::Referenced* _ptr;
    };

    /** Smart pointer for observed objects, that automatically set pointers to them to null when they are deleted.
      * To use the ObsPtr<> robustly in multi-threaded applications it is recommend to access the pointer via
      * the lock() method that passes back a RefPtr<> that safely takes a reference to the object to prevent deletion
      * during usage of the object.  In certain conditions it may be safe to use the pointer directly without using lock(),
      * which will confer a perfomance advantage, the conditions are:
      *   1) The data structure is only accessed/deleted in single threaded/serial way.
      *   2) The data strucutre is guarenteed by high level management of data strucutures and threads which avoid
      *      possible situations where the ObsPtr<>'s object may be deleted by one thread whilst being accessed
      *      by another.
      * If you are in any doubt about whether it is safe to access the object safe then use the
      * RefPtr<> ObsPtr<>.lock() combination. */
    template<class T>
    class ObsPtr
    {
    public:
        typedef T element_type;
        ObsPtr() : _reference(0), _ptr(0) {}

        /**
         * Create a ObsPtr from a RefPtr.
         */
        ObsPtr(const RefPtr<T>& rp)
        {
            _reference = rp.valid() ? rp->getOrCreateObserverSet() : 0;
            _ptr = (_reference.valid() && _reference->getObserverdObject()!=0) ? rp.get() : 0;
        }

        /**
         * Create a ObsPtr from a raw pointer. For compatibility;
         * the result might not be lockable.
         */
        ObsPtr(T* rp)
        {
            _reference = rp ? rp->getOrCreateObserverSet() : 0;
            _ptr = (_reference.valid() && _reference->getObserverdObject()!=0) ? rp : 0;
        }

        ObsPtr(const ObsPtr& wp) :
            _reference(wp._reference),
            _ptr(wp._ptr)
        {
        }

        ~ObsPtr()
        {
        }

        ObsPtr& operator = (const ObsPtr& wp)
        {
            if (&wp==this) return *this;

            _reference = wp._reference;
            _ptr = wp._ptr;
            return *this;
        }

        ObsPtr& operator = (const RefPtr<T>& rp)
        {
            _reference = rp.valid() ? rp->getOrCreateObserverSet() : 0;
            _ptr = (_reference.valid() && _reference->getObserverdObject()!=0) ? rp.get() : 0;
            return *this;
        }

        ObsPtr& operator = (T* rp)
        {
            _reference = rp ? rp->getOrCreateObserverSet() : 0;
            _ptr = (_reference.valid() && _reference->getObserverdObject()!=0) ? rp : 0;
            return *this;
        }

        /**
         * Assign the ObsPtr to a RefPtr. The RefPtr will be valid if the
         * referenced object hasn't been deleted and has a ref count > 0.
         */
        bool lock(RefPtr<T>& rptr) const
        {
            if (!_reference)
            {
                rptr = 0;
                return false;
            }

            Referenced* obj = _reference->addRefLock();
            if (!obj)
            {
                rptr = 0;
                return false;
            }

            rptr = _ptr;
            obj->unref_nodelete();
            return rptr.valid();
        }

        /** Comparison operators. These continue to work even after the
         * observed object has been deleted.
         */
        bool operator == (const ObsPtr& wp) const { return _reference == wp._reference; }
        bool operator != (const ObsPtr& wp) const { return _reference != wp._reference; }
        bool operator < (const ObsPtr& wp) const { return _reference < wp._reference; }
        bool operator > (const ObsPtr& wp) const { return _reference > wp._reference; }

        // Non-strict interface, for compatibility
        // comparison operator for const T*.
        inline bool operator == (const T* ptr) const { return _ptr == ptr; }
        inline bool operator != (const T* ptr) const { return _ptr != ptr; }
        inline bool operator < (const T* ptr) const { return _ptr < ptr; }
        inline bool operator > (const T* ptr) const { return _ptr > ptr; }

        // Convenience methods for operating on object, however, access is not automatically threadsafe.
        // To make thread safe, one should either ensure at a high level
        // that the object will not be deleted while operating on it, or
        // by using the ObsPtr<>::lock() to get a RefPtr<> that
        // ensures the objects stay alive throughout all access to it.

        // Throw an error if _reference is null?
        inline T& operator*() const { return *static_cast<T*>(_ptr); }
        inline T* operator->() const { return static_cast<T*>(_ptr); }

        // get the raw C pointer
        inline T* get() const { return (_reference.valid() && _reference->getObserverdObject()!=0) ? static_cast<T*>(_ptr) : 0; }

        inline bool operator!() const   { return get() == 0; }
        inline bool valid() const       { return get() != 0; }

    protected:

        RefPtr<osg::ObserverSet>   _reference;
        Referenced*                _ptr;
    };
#else

    /*!
     \short This class provides Reference counting objects. Instances should be used by the RefPtr class
     */
    class STEP_EXPORT Referenced
    {
        ClassType_definitions();

    public:

        /// Default constructor
        Referenced();

        /// Copy Constructor
        Referenced(const Referenced&);

        /// Overloaded assignment operator
        inline Referenced& operator =(const Referenced&)
        {
            return *this;
        }

        /*!
         \short Explicitly increment the reference count by one.
         Increment the reference count by one, indicating that
         this object has another pointer which is referencing it.
         */
        inline void ref() const
        {
            ++_refCount;
        }

        /*!
         \short Explicitly  decrement the reference count by one.
         Decrement the reference count by one, indicating that
         a pointer to this object is referencing it.  If the
         reference count goes to zero, it is assumed that this object
         is no longer referenced and is automatically deleted.
         */
        inline void unref() const
        {
            {
                --_refCount;
            }
            if (_refCount <= 0)
                delete this;
        }

        /*! decrement the reference count by one, indicating that
         a pointer to this object is referencing it.  However, do
         not delete it, even if ref count goes to 0.  Warning, unref_nodelete()
         should only be called if the user knows exactly who will
         be responsible for, one should prefer unref() over unref_nodelete()
         as the later can lead to memory leaks.
         */
        void unref_nodelete() const;

        /*!
         \short return the number pointers currently referencing this object.
         */
        inline int referenceCount() const
        {
            return _refCount;
        }

        /**
         *  Add a Observer that is observing this object, notify the Observer when this object gets deleted.
         *  \param observer The observer to add.
         */
        void addObserver(Observer* observer);

        /**
         *  Add a Observer that is observing this object, notify the Observer when this object gets deleted.
         *  \param observer The observer to remove.
         */
        void removeObserver(Observer* observer);

    protected:
        virtual ~Referenced();

    private:
        mutable int _refCount;
        void * _observers;

    };


    /*!
     * \short This class provides the means to keep a reference on Referenced objects
     *
     * example of use :
     * \code
     * class A : public Referenced {  [...] }
     *
     * RefPtr<A> a1 = new A;    // a1->referenceCount() == 1;
     * RefPtr<A> a2 = a1.get(); // a2->referenceCount == 2;
     * a2 = 0;                  // a1->referenceCount == 1;
     * a1 = 0;                  // A::~A() is called;
     * \endcode
     */
    template<class T>
    class RefPtr
    {
    public:
        //! a typedef to our element type
        typedef T element_type;

        //! default constructor
        RefPtr() :
            _ptr(NULL)
        {
        }

        /*!
         * constructor with a pointer to the object to reference
         * \param ptr the object to keep a reference on
         */
        RefPtr(Referenced * ptr) :
            _ptr(ptr)
        {
            if (_ptr)
                _ptr->ref();
        }

        /*!
         * constructor with a reference to the object to reference
         * \param rp the reference of the object to keep a reference on
         */
        RefPtr(const RefPtr& rp) :
            _ptr(rp._ptr)
        {
            if (_ptr)
                _ptr->ref();
        }

        /*!
         * constructor with a reference (of another type, must be a subtype) to the object to reference
         * \param rp the reference of the object to keep a reference on
         */
        template<class T2>
        RefPtr<T> (const RefPtr<T2>& rp) :
            _ptr(rp.get())
        {
            if (_ptr)
                _ptr->ref();
        }

        ~RefPtr()
        {
            if (_ptr)
                _ptr->unref();
            _ptr = NULL;
        }

        /*!
         * Assignment operator with a reference to the object to reference
         * \param rp the reference of the object to keep a reference on
         */
        RefPtr& operator =(const RefPtr& rp)
        {
            if (_ptr == rp._ptr)
                return *this;
            Referenced* tmp_ptr = _ptr;
            _ptr = rp._ptr;
            if (_ptr)
                _ptr->ref();
            // unref second to prevent any deletion of any object which might
            // be Referenced by the other object. i.e rp is child of the
            // original _ptr.
            if (tmp_ptr)
                tmp_ptr->unref();
            return *this;
        }

        /*!
         * Assignment operator with a pointer to the object to reference
         * \param ptr the pointer of the object to keep a reference on
         */
        RefPtr& operator =(T* ptr)
        {
            if (_ptr == ptr)
                return *this;
            Referenced* tmp_ptr = _ptr;
            _ptr = ptr;
            if (_ptr)
                _ptr->ref();
            // unref second to prevent any deletion of any object which might
            // be Referenced by the other object. i.e rp is child of the
            // original _ptr.
            if (tmp_ptr)
                tmp_ptr->unref();
            return *this;
        }

        /*!
         * Assignement with a reference (of another type, must be a subtype) to the object to reference
         * \param rp the reference of the object to keep a reference on
         */
        template<class T2>
        RefPtr<T>& operator=(const RefPtr<T2>& rp)
        {
            if (_ptr == rp.get())
                return *this;
            Referenced* tmp_ptr = _ptr;
            _ptr = rp.get();
            if (_ptr)
                _ptr->ref();
            // unref second to prevent any deletion of any object which might
            // be Referenced by the other object. i.e rp is child of the
            // original _ptr.
            if (tmp_ptr)
                tmp_ptr->unref();
            return *this;
        }
        /*!
         * Assignement with a pointer (of another type, must be a subtype) to the object to reference
         * \param ptr the reference of the object to keep a reference on
         */

        template<class T2>
        RefPtr<T>& operator=(T2* ptr)
        {
            if (_ptr == ptr)
                return *this;
            Referenced* tmp_ptr = _ptr;
            _ptr = ptr;
            if (_ptr)
                _ptr->ref();
            // unref second to prevent any deletion of any object which might
            // be Referenced by the other object. i.e rp is child of the
            // original _ptr.
            if (tmp_ptr)
                tmp_ptr->unref();
            return *this;
        }

        /*!
         * comparison operators for RefPtr. It compares the Referenced pointers
         * \param rhs RefPtr to compare to
         * \{
         */
        inline bool operator ==(const RefPtr& rhs) const
        {
            return (_ptr == rhs._ptr);
        }
        inline bool operator !=(const RefPtr& rhs) const
        {
            return (_ptr != rhs._ptr);
        }
        inline bool operator <(const RefPtr& rhs) const
        {
            return (_ptr < rhs._ptr);
        }
        inline bool operator >(const RefPtr& rhs) const
        {
            return (_ptr > rhs._ptr);
        }

        //! \}

        /*!
         *  comparison operator for const T*.
         *  \param ptr pointer to compare to
         * \{
         */
        inline bool operator ==(const T* ptr) const
        {
            return (_ptr == ptr);
        }
        inline bool operator !=(const T* ptr) const
        {
            return (_ptr != ptr);
        }
        inline bool operator <(const T* ptr) const
        {
            return (_ptr < ptr);
        }
        inline bool operator >(const T* ptr) const
        {
            return (_ptr > ptr);
        }
        //! \}

        /*!
         *  getters
         * \{
         */
        T& operator*() const
        {
            return *(static_cast<T*> (_ptr));
        }
        T* operator->() const
        {
            return static_cast<T*> (_ptr);
        }
        T* get() const
        {
            return static_cast<T*> (_ptr);
        }
        //! \}

        /*!
         *  to test the validity of the reference
         * \{
         */
        bool operator!() const
        {
            return _ptr == NULL;
        }

        bool valid() const
        {
            return _ptr != NULL;
        }
        //! \}

        /*!
         * take control over the object pointed to by RefPtr, unReferenced but do not delete even if ref count goes to 0,
         * return the pointer to the object.
         * Note, do not use this unless you are 100% sure your code handles the deletion of the object correctly, and
         * only use when absolutely required.
         */
        T* release()
        {
            T* tmp = static_cast<T*> (_ptr);
            if (_ptr)
            {
                _ptr->unref_nodelete();
            }
            _ptr = NULL;
            return tmp;
        }

        /*!
         * to exchange two Referenced objects
         * \param rp the RefPtr to swap with
         */
        void swap(RefPtr& rp)
        {
            T* tmp = _ptr;
            _ptr = rp._ptr;
            rp._ptr = tmp;
        }

    private:
        //! the pointer to our referenced object
        Referenced* _ptr;
    };
    /*!
     \short This class provides a common interface to all observers
     */
    class Observer
    {
    public:
        virtual ~Observer()
        {
        }

        /*!
         * Called by Referenced::~Referenced().
         * Implement to perform some actions when this objects gets deleted.
         */
        virtual void objectDeleted(void*) = 0;
    };

    /*!
     *  Smart pointer for observed objects, that automatically set pointers to them to null when they deleted.
     */
    template<class T>
    class ObsPtr: public Observer
    {

    public:
        //! typedef to our element_type
        typedef T element_type;

        ObsPtr() : _ptr(0L)
        {
        }
        /*!
         * contructor
         * \param t pointer to a Referenced object
         */
        ObsPtr(Referenced* t) : _ptr(t)
        {
            if (_ptr)
                _ptr->addObserver(this);
        }

        /*!
         * copy contructor
         * \param rp reference to the ObsPtr to copy
         */
        ObsPtr(const ObsPtr& rp) :
            Observer(), _ptr(rp._ptr)
        {
            if (_ptr)
                _ptr->addObserver(this);
        }
        ~ObsPtr()
        {
            if (_ptr)
                _ptr->removeObserver(this);
            _ptr = 0;
        }

        /*!
         * Assignment operator
         * \param rhs the ObsPtr to assign
         * \return a reference to this ObsPtr
         */
        inline ObsPtr& operator =(const ObsPtr& rhs)
        {
            if (_ptr == rhs._ptr)
            {
                return *this;
            }
            if (_ptr)
            {
                _ptr->removeObserver(this);
            }

            _ptr = rhs._ptr;

            if (_ptr)
            {
                _ptr->addObserver(this);
            }
            return *this;
        }

        /*!
         * Assignment operator
         * \param ptr the Referenced object to assign
         * \return a reference to this ObsPtr
         */
        inline ObsPtr& operator =(Referenced* ptr)
        {
            if (_ptr == ptr)
            {
                return *this;
            }
            if (_ptr)
            {
                _ptr->removeObserver(this);
            }

            _ptr = ptr;

            if (_ptr)
            {
                _ptr->addObserver(this);
            }
            return *this;
        }

        /*!
         * remove our reference to the Referenced object
         */
        virtual void objectDeleted(void*)
        {
            _ptr = 0;
        }

        /*!
         * comparison operators for ObsPtr. It compares the Referenced pointers
         * \param rhs ObsPtr to compare to
         * \return success
         * \{
         */
        inline bool operator ==(const ObsPtr& rhs) const
        {
            return (_ptr == rhs._ptr);
        }
        inline bool operator !=(const ObsPtr& rhs) const
        {
            return (_ptr != rhs._ptr);
        }
        inline bool operator <(const ObsPtr& rhs) const
        {
            return (_ptr < rhs._ptr);
        }
        inline bool operator >(const ObsPtr& rhs) const
        {
            return (_ptr > rhs._ptr);
        }
        //! \}

        /*!
         * comparison operator for const T*.
         * \param ptr pointer to compare to
         * \return success
         * \{
         */
        inline bool operator ==(const T* ptr) const
        {
            return (_ptr == ptr);
        }
        inline bool operator !=(const T* ptr) const
        {
            return (_ptr != ptr);
        }
        inline bool operator <(const T* ptr) const
        {
            return (_ptr < ptr);
        }
        inline bool operator >(const T* ptr) const
        {
            return (_ptr > ptr);
        }
        //! \}

        /*!
         *  getters
         * \{
         */
        inline T& operator*()
        {
            return *static_cast<T*> (_ptr);
        }

        inline const T& operator*() const
        {
            return *static_cast<T*> (_ptr);
        }

        inline T* operator->()
        {
            return static_cast<T*> (_ptr);
        }

        inline const T* operator->() const
        {
            return static_cast<T*> (_ptr);
        }

        inline T* get()
        {
            return static_cast<T*> (_ptr);
        }

        inline const T* get() const
        {
            return static_cast<T*> (_ptr);
        }
        //! \}

        /*!
         *  to test the validity of the reference
         * \{
         */
        inline bool valid() const
        {
            return _ptr != 0L;
        }

        inline bool operator!() const
        {
            return _ptr == 0L;
        }
        //! \}

    private:
        Referenced* _ptr;
    };

#endif
}
#endif
