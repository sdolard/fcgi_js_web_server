#ifndef HTTPTRANSACTION_LIST_H
#define HTTPTRANSACTION_LIST_H

#include <QByteArray>
#include <QSharedPointer>
#include <QQueue>

#include "http_transaction.h"


class HttpTransactionList
{
public:
    // Returns current pending transaction if exists or create a new one
    QSharedPointer<HttpTransaction> getReadingTransaction();

    // Returns current writing transaction of exists
    // Returns an empty HttpTransaction if not exists > this should not occured
    QSharedPointer<HttpTransaction> getWritingTransaction();

    // Swap current reading transaction state (HttpTransaction::stReadingHeader or a HttpTransaction::stReadMessageBody)
    // to HttpTransaction::stWriteHeader state
    // Assert if there is no current reading transaction state
    // Internal move HttpTransaction to m_readingList or m_writingList
    void swapReadingTransactionToWritingState();

    // Transaction is complete
    // We are done
    void dequeueWritingTransaction();
private:
    QQueue< QSharedPointer<HttpTransaction> > m_readingList;
    QQueue< QSharedPointer<HttpTransaction> > m_writingList;
};

#endif // HTTPTRANSACTION_LIST_H
