#pragma once

#ifndef WINUTIL_IPC_HPP
#define WINUTIL_IPC_HPP

#include "IPC.h"

#ifdef __cplusplus

#include <iostream>
//#include <fstream>

namespace 
{
	namespace IPC
	{
		class pipe_closed : public std::exception
		{
		public:
			pipe_closed() = default;
			~pipe_closed() = default;
			const char* what() const noexcept override
			{
				return "pipe already closed or not set.";
			}
		};

		class pipebuf : public std::streambuf
		{
		public:
			pipebuf()
			{
				gbuf = new char[1024];
				pbuf = new char[1024];
				setg(gbuf, gbuf, gbuf);
				setp(pbuf, pbuf, pbuf + 1024);
				hpipe = nullptr;
			}
			~pipebuf()
			{
				delete[] gbuf;
				delete[] pbuf;
			}
			HPIPE set_pipe(HPIPE pipe_handle = nullptr)
			{
				HPIPE last_pipe = hpipe;
				hpipe = pipe_handle;
				return last_pipe;
			}
		protected:
			int_type overflow(int_type ch = traits_type::eof()) override
			{
				if (hpipe == nullptr) throw pipe_closed();
				char* str = pbase();
				if (PipeWrite(hpipe, pbase(), pptr() - pbase()) == 0)
				{
					setp(pbuf, nullptr, pbuf + 1024);
					throw pipe_closed();
				}
				setp(pbuf, pbuf, pbuf + 1024);
				return 0;
			}
			int_type underflow() override
			{
				if (hpipe == nullptr) throw pipe_closed();
				size_t getsize = PipeRead(hpipe, gbuf, 1024);
				if (getsize == 0)
				{
					setg(gbuf, nullptr, gbuf + 1024);
					throw pipe_closed();
				}
				setg(gbuf, gbuf, gbuf + getsize);
				return *gptr();
			}
			int sync() override
			{
				if (hpipe == nullptr) return -1;
				if (pptr() == pbase()) return 0;
				try { overflow(traits_type::eof()); }
				catch (...) { return -1; }
				return 0;
			}
		private:
			char* gbuf;
			char* pbuf;
			HPIPE hpipe;
		};

		class ipipe : public std::istream
		{
		public:
			ipipe() : std::istream(nullptr)
			{
				mybuf = new pipebuf;
				hpipe = nullptr;
				init(mybuf);
			}
			~ipipe()
			{
				delete mybuf;
			}
			HPIPE create(std::string pipe_name)
			{
				hpipe = PipeCreateA(pipe_name.c_str(), std::ios::in);
				return hpipe;
			}
			HPIPE create(std::wstring pipe_name)
			{
				hpipe = PipeCreateW(pipe_name.c_str(), std::ios::in);
				return hpipe;
			}
			void open()
			{
				if (hpipe == nullptr) return;
				if (PipeOpen(hpipe))
				{
					mybuf->set_pipe(hpipe);
					clear();
					return;
				}
				setstate(badbit);
			}
			void close()
			{
				if (hpipe == nullptr) return;
				PipeClose(hpipe);
				mybuf->set_pipe();
				setstate(badbit);
			}
			HPIPE connect(std::string pipe_name)
			{
				hpipe = PipeConnectA(pipe_name.c_str(), std::ios::in);
				mybuf->set_pipe(hpipe);
				clear();
				return hpipe;
			}
			HPIPE connect(std::wstring pipe_name)
			{
				hpipe = PipeConnectW(pipe_name.c_str(), std::ios::in);
				mybuf->set_pipe(hpipe);
				clear();
				return hpipe;
			}
			void destroy()
			{
				if (hpipe == nullptr) return;
				PipeDestroy(hpipe);
				hpipe = nullptr;
				mybuf->set_pipe();
				setstate(badbit);
			}
		private:
			pipebuf* mybuf;
			HPIPE hpipe;
		};

		class opipe : public std::ostream
		{
		public:
			opipe() : std::ostream(nullptr)
			{
				mybuf = new pipebuf;
				hpipe = nullptr;
				init(mybuf);
			}
			~opipe()
			{
				delete mybuf;
			}
			HPIPE create(std::string pipe_name)
			{
				hpipe = PipeCreateA(pipe_name.c_str(), std::ios::out);
				return hpipe;
			}
			HPIPE create(std::wstring pipe_name)
			{
				hpipe = PipeCreateW(pipe_name.c_str(), std::ios::out);
				return hpipe;
			}
			void open()
			{
				if (hpipe == nullptr) return;
				if (PipeOpen(hpipe))
				{
					mybuf->set_pipe(hpipe);
					clear();
					return;
				}
				setstate(badbit);
			}
			void close()
			{
				if (hpipe == nullptr) return;
				PipeClose(hpipe);
				mybuf->set_pipe();
				setstate(badbit);
			}
			HPIPE connect(std::string pipe_name)
			{
				hpipe = PipeConnectA(pipe_name.c_str(), std::ios::out);
				mybuf->set_pipe(hpipe);
				clear();
				return hpipe;
			}
			HPIPE connect(std::wstring pipe_name)
			{
				hpipe = PipeConnectW(pipe_name.c_str(), std::ios::out);
				mybuf->set_pipe(hpipe);
				clear();
				return hpipe;
			}
			void destroy()
			{
				if (hpipe == nullptr) return;
				PipeDestroy(hpipe);
				hpipe = nullptr;
				mybuf->set_pipe();
				setstate(badbit);
			}
		private:
			pipebuf* mybuf;
			HPIPE hpipe;
		};

		class pipe : public std::iostream
		{
		public:
			pipe() : std::iostream(nullptr)
			{
				mybuf = new pipebuf;
				hpipe = nullptr;
				init(mybuf);
			}
			~pipe()
			{
				delete mybuf;
			}
			HPIPE create(std::string pipe_name, int access_mode)
			{
				hpipe = PipeCreateA(pipe_name.c_str(), access_mode);
				return hpipe;
			}
			HPIPE create(std::wstring pipe_name, int access_mode)
			{
				hpipe = PipeCreateW(pipe_name.c_str(), access_mode);
				return hpipe;
			}
			void open()
			{
				if (hpipe == nullptr) return;
				if (PipeOpen(hpipe))
				{
					mybuf->set_pipe(hpipe);
					clear();
					return;
				}
				setstate(badbit);
			}
			void close()
			{
				if (hpipe == nullptr) return;
				PipeClose(hpipe);
				mybuf->set_pipe();
				setstate(badbit);
			}
			HPIPE connect(std::string pipe_name, int access_mode)
			{
				hpipe = PipeConnectA(pipe_name.c_str(), access_mode);
				mybuf->set_pipe(hpipe);
				clear();
				return hpipe;
			}
			HPIPE connect(std::wstring pipe_name, int access_mode)
			{
				hpipe = PipeConnectW(pipe_name.c_str(), access_mode);
				mybuf->set_pipe(hpipe);
				clear();
				return hpipe;
			}
			void destroy()
			{
				if (hpipe == nullptr) return;
				PipeDestroy(hpipe);
				hpipe = nullptr;
				mybuf->set_pipe();
				setstate(badbit);
			}
		private:
			pipebuf* mybuf;
			HPIPE hpipe;
		};
	}
}
#endif // __cplusplus

#endif // !WINUTIL_IPC_HPP
