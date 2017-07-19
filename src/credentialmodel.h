#ifndef CREDENTIALMODEL_H
#define CREDENTIALMODEL_H

// Qt
#include <QAbstractItemModel>
#include <QJsonArray>
#include <QDate>

// Application
#include "Common.h"
class RootItem;
class ServiceItem;
class TreeItem;

class CredentialModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Enumerations
    //-------------------------------------------------------------------------------------------------

    enum ColumnIdx
    {
        ServiceIdx,
        LoginIdx,
        PasswordIdx,
        DescriptionIdx,
        DateCreatedIdx,
        DateModifiedIdx,
        FavoriteIdx,
        ColumnCount,
    };

    enum CustomRole {
        LoginRole = Qt::UserRole + 1,
        PasswordUnlockedRole,
        FavRole,
        UidRole
    };

    //-------------------------------------------------------------------------------------------------
    // Credential descriptor
    //-------------------------------------------------------------------------------------------------

    class Credential
    {
    public:
        QString service;
        QString login;
        QString password, passwordOrig;
        QString description;
        QDate createdDate;
        QDate updatedDate;
        QByteArray address;
        qint8 favorite = Common::FAV_NOT_SET;

        bool operator==(const Credential &other) const
        {
            if (address.isEmpty() || other.address.isEmpty())
            {
                //if address is defined for both, check equality of address
                return address == other.address;
            }
            return service == other.service && login == other.login;
        }
    };

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CredentialModel(QObject *parent=nullptr);

    //! Destructor
    virtual ~CredentialModel();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Data
    virtual QVariant data(const QModelIndex &index, int role) const;

    //! Return flags
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    //! Index
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    //! Parent
    virtual QModelIndex parent(const QModelIndex &index) const;

    //! Row count
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //! Column count
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    //! Load
    void load(const QJsonArray &json);

    //! Set clear text passowrd
    void setClearTextPassword(const QString &sServiceName, const QString &sLoginName, const QString &sPassword);

    //! Update
    void update(const QString &sServiceName, const QString &sLoginName, const QString &sPassowrd, const QString &sDescription);

    //! Update
    void update(const QModelIndex &idx, const Credential &cred);

    //! Get JSON changes
    QJsonArray getJsonChanges();

    //! Remove credential
    void removeCredential(const QModelIndex &idx);

    //! Get item
    TreeItem *getItem(const QModelIndex &index) const;

private:
    //! Add service
    ServiceItem *addService(const QString &sServiceName);

    //! Return index for item UID
    QModelIndex findIndexForItemUID(const QString &sItemUID) const;

private:
    //! Root item
    RootItem *m_pRootItem;

signals:
    //! Model loaded
    void modelLoaded();
};

#endif // CREDENTIALMODEL_H