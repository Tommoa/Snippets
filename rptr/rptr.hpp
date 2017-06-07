#ifndef _SNIPPETS_RPTR_DEF_HPP
#define _SNIPPETS_RPTR_DEF_HPP
#include <string>
#include <cstdlib>

namespace Snippets {

    /**
    *  rptr<T, S=int, A=long>
    *  A class that stores a pointer of as a signed number of type S to a type T. "A" refers to the size of datatype that can store all of the addresses in the machine.
    *
    *  Abuses the fact that alignment is forced to multiples of 4 bytes by all modern systems.
    *  Points to itself if it is null.
    **/
    template <typename T, typename S = int, typename A = long> class rptr {
      private:
        S to;
      public:
        rptr() {
            this->to = (S)0;
        }
        rptr(T* to) {
            this->to = (S)((A)to - (A)this) ;
        }
        rptr(T* to, void* base) {
            this->to = (S)((A)to - (A)base) + 1;
        }
        rptr(T* to, A base) {
            this->to = (S)((A) to - base) + 1;
        }

        void change(T* to) {
            this->to = (S)((A)to - (A)this);
        }
        void change(T* to, void* base) {
            this->to = (S)((A)to - (A)base) + 1;
        }
        void change(T* to, A base) {
            this->to = (S)((A)to - base) + 1;
        }

        std::string print() {
            std::string ret = "Offset of ";
            ret += std::to_string(to&1 ? to-1 : to);
            ret += " relative to ";
            ret += (to&1 ? "a base" : "self");
            return ret;
        }

        void assign(T what) {
            T* thing = (T*)((A)this + to);
            *thing = what;
        }

        void assign(T what, void* base) {
            T* thing = (T*)((A)base + to-1);
            *thing = what;
        }

        void assign(T what, A base) {
            T* thing = (T*)(base + to-1);
            *thing = what;
        }

        void operator=(rptr other) {
            if (other.to&1)
                this->to = (S)(other.to);
            else
                this->to = (S)(other.to + ((A)&other - (A)this));
        }
        void operator=(T *other) {
            this->to = (S)((A)other - (A)this);
        }

        T operator*() {
            if (to&1)
                throw;
            return *(T*)((A)this + to);
        }
        T operator*(void* base) {
            if (!(to&1))
                throw;
            return *(T*)((A)base + to-1);
        }
        T operator*(A base) {
            if (!(to&1))
                throw;
            return *(T*)((base + to)-1);
        }
    };
}
#endif
