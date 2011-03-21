#include "http_transaction_list.h"


QSharedPointer<HttpTransaction> HttpTransactionList::getReadingTransaction()
{
    if (m_readingList.isEmpty())
    {
        // New transaction
        QSharedPointer<HttpTransaction> t = QSharedPointer<HttpTransaction>(new HttpTransaction());
        m_readingList.enqueue(t);
        qDebug("getReadingTransaction: HttpTransaction created (total 1)");
        return t;
    }
    return m_readingList.head();
}

QSharedPointer<HttpTransaction> HttpTransactionList::getWritingTransaction()
{
    Q_ASSERT(!m_writingList.isEmpty());
    if (m_writingList.isEmpty())
        return QSharedPointer<HttpTransaction>(new HttpTransaction());

    return m_writingList.head();
}

void HttpTransactionList::swapReadingTransactionToWritingState()
{
    Q_ASSERT(!m_readingList.isEmpty());
    QSharedPointer<HttpTransaction> transaction = m_readingList.dequeue();

    Q_ASSERT(transaction->state() == HttpTransaction::tsReadHeader ||
             transaction->state() == HttpTransaction::tsReadMessageBody);

    transaction->setState(HttpTransaction::tsWriteHeader);
    m_writingList.enqueue(transaction);
}

void HttpTransactionList::dequeueWritingTransaction()
{
    Q_ASSERT(!m_writingList.isEmpty());
    m_writingList.dequeue();
}
