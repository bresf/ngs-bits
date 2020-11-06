#include "SessionManager.h"

SessionManager::SessionManager()
	: session_store_()
{
}

SessionManager& SessionManager::instance()
{
	static SessionManager session_manager;
	return session_manager;
}

void SessionManager::addNewSession(QString id, Session in)
{
	instance().mutex_.lock();
	instance().session_store_.insert(id, in);
	instance().mutex_.unlock();
}

void SessionManager::removeSession(QString id)
{
	if (instance().session_store_.contains(id))
	{
		instance().mutex_.lock();
		instance().session_store_.remove(id);
		instance().mutex_.unlock();
	}
}

Session SessionManager::getSessionByUserId(QString id)
{
	QMapIterator<QString, Session> i(instance().session_store_);
	while (i.hasNext()) {
		i.next();
		if (i.value().user_id == id)
		{
			return i.value();
		}
	}
	return Session{};
}

bool SessionManager::hasValidToken(QString id)
{
	Session found_session = getSessionByUserId(id);
	qint64 login_time = found_session.login_time.toSecsSinceEpoch();
	//qint64 valid_period = 60*60*35; // 35 hours in seconds
	qint64 valid_period = 10;

	if ((login_time + valid_period) > QDateTime::currentDateTime().toSecsSinceEpoch())
	{
		return true;
	}
	return false;
}
