#pragma once

#include <memory>
#include <unordered_map>
#include "buff/Buff.h"
#include "Sock.h"

namespace fstServ {

	class SocketHandler;

	class StreamSocket {

	public:

		class IState {

		public:

			~IState() {}
			// 从soket读取数据到缓冲区.
			virtual void read(StreamSocket&, SocketHandler&) = 0;
		};

		class UnReadyState : public IState {

		public:

			void read(StreamSocket&, SocketHandler&) override;
		};

		class ReadyState : public IState {

		public:

			void read(StreamSocket&, SocketHandler&) override;
		};

	public:

		// 套接字中的事件状态.
		enum EventStatus { EV_NONE = 0U, EV_RD = 1U, EV_WR = 2U, EV_EX = 4U };

		enum BuffStatus { BUFF_NONE, BUFF_RD_READY = 1U, BUFF_WR_READY = 2U };

		StreamSocket(socket_t fd);

		~StreamSocket();

		StreamSocket(StreamSocket&&) noexcept;

		StreamSocket& operator==(StreamSocket&&);

		void swap(StreamSocket&);

		socket_t getFd() const;

		void setEvents(const unsigned);

		unsigned getEvents() const;

		void setBuffStatus(const unsigned);

		unsigned getBuffStatus() const;

		// 重置套接字事件状态.
		void reset();

		// 重置缓存状态.
		void resetBuffStatus();

		bool operator==(const StreamSocket&) const;

		void write(const char*, size_t);

		void read(SocketHandler&);

		Buff& getReadBuff();

		Buff& getWriteBuff();

		// 获取当前套接字读取状态
		IState& getState();

	private:

		socket_t m_fd;

		unsigned m_events;

		unsigned m_buffStatus;

		std::unique_ptr<Buff> m_buffRead;

		std::unique_ptr<Buff> m_buffWrite;

		static ReadyState m_ready;

		static UnReadyState m_unReady;

	};

	typedef std::shared_ptr<StreamSocket> LpSocket;

	using SSockSet = std::unordered_map<socket_t, LpSocket>;

}