#ifndef IINFORMATIONRESOURCE_H
#define IINFORMATIONRESOURCE_H

#include <QObject>
#include <QString>

#include "IIterator.h"

template<class TResource>
class IteratorResolverBase;

class IIteratorResolver
{
	public:
		virtual ~IIteratorResolver() = default;

		template<class TResource>
		IIterator<TResource> *resolve()
		{
			auto resolver = dynamic_cast<IteratorResolverBase<TResource> *>(this);

			if (resolver)
			{
				return resolver->resolve();
			}

			return nullptr;
		}
};

template<class TResource>
class IteratorResolverBase : public IIteratorResolver
{
	public:
		virtual IIterator<TResource> *resolve() = 0;
};

template<class TInstance, class TIterator>
class IteratorResolver : public IteratorResolverBase<typename TIterator::TResource>
{
	public:
		IteratorResolver(TInstance *instance)
			: m_iterator(instance)
		{

		}

		IIterator<typename TIterator::TResource> *resolve() override
		{
			return &m_iterator;
		}

	private:
		TIterator m_iterator;
};

class IInformationResource : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString id READ id CONSTANT)

	public:
		virtual QString id() const = 0;
		virtual QString key() const = 0;
		virtual QString url() const = 0;

		template<class TResource>
		IIterator<TResource> *iterator()
		{
			for (IIteratorResolver *resolver : m_iterators)
			{
				IIterator<TResource> *iterator = resolver->resolve<TResource>();

				if (iterator)
				{
					return iterator;
				}
			}

			return nullptr;
		}

	protected:
		QList<IIteratorResolver *> m_iterators;
};

template<class T>
class InformationResource : public IInformationResource
{
	public:
		InformationResource()
			: m_fetched(false)
		{

		}

	protected:
		template<class TIterator>
		void registerIterator()
		{
			m_iterators << new IteratorResolver<T, TIterator>((T *)this);
		}

		template<class TReturn>
		TReturn property(TReturn T::*accessor)
		{
			static const TReturn DefaultValue = TReturn();

			const T *instance = static_cast<T *>(this);
			const TReturn &field = (instance->*accessor);

			if (field == DefaultValue)
			{
				if (!m_fetched)
				{
					m_fetched = true;

					fetchExtendedInfo();

					return (instance->*accessor);
				}
			}

			return field;
		}

		virtual void fetchExtendedInfo()
		{

		}

	private:
		bool m_fetched;
};

#endif // IINFORMATIONRESOURCE_H
