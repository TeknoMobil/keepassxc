/*
 *  Copyright (C) 2010 Felix Geyer <debfx@fobos.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KEEPASSX_METADATA_H
#define KEEPASSX_METADATA_H

#include <QColor>
#include <QDateTime>
#include <QHash>
#include <QImage>
#include <QPixmap>
#include <QPixmapCache>
#include <QPointer>
#include <QUuid>

#include "core/CustomData.h"

class Database;
class Group;

class Metadata : public QObject
{
    Q_OBJECT

public:
    explicit Metadata(QObject* parent = nullptr);

    struct MetadataData
    {
        QString generator;
        QString name;
        QDateTime nameChanged;
        QString description;
        QDateTime descriptionChanged;
        QString defaultUserName;
        QDateTime defaultUserNameChanged;
        int maintenanceHistoryDays;
        QColor color;
        bool recycleBinEnabled;
        int historyMaxItems;
        int historyMaxSize;
        int masterKeyChangeRec;
        int masterKeyChangeForce;

        bool protectTitle;
        bool protectUsername;
        bool protectPassword;
        bool protectUrl;
        bool protectNotes;
    };

    QString generator() const;
    QString name() const;
    QDateTime nameChanged() const;
    QString description() const;
    QDateTime descriptionChanged() const;
    QString defaultUserName() const;
    QDateTime defaultUserNameChanged() const;
    QDateTime settingsChanged() const;
    int maintenanceHistoryDays() const;
    QColor color() const;
    bool protectTitle() const;
    bool protectUsername() const;
    bool protectPassword() const;
    bool protectUrl() const;
    bool protectNotes() const;
    QImage customIcon(const QUuid& uuid) const;
    QPixmap customIconPixmap(const QUuid& uuid) const;
    QPixmap customIconScaledPixmap(const QUuid& uuid) const;
    bool containsCustomIcon(const QUuid& uuid) const;
    QHash<QUuid, QImage> customIcons() const;
    QList<QUuid> customIconsOrder() const;
    bool recycleBinEnabled() const;
    QHash<QUuid, QPixmap> customIconsScaledPixmaps() const;
    Group* recycleBin();
    const Group* recycleBin() const;
    QDateTime recycleBinChanged() const;
    const Group* entryTemplatesGroup() const;
    QDateTime entryTemplatesGroupChanged() const;
    const Group* lastSelectedGroup() const;
    const Group* lastTopVisibleGroup() const;
    QDateTime masterKeyChanged() const;
    int masterKeyChangeRec() const;
    int masterKeyChangeForce() const;
    int historyMaxItems() const;
    int historyMaxSize() const;
    CustomData* customData();
    const CustomData* customData() const;

    static const int DefaultHistoryMaxItems;
    static const int DefaultHistoryMaxSize;

    void setGenerator(const QString& value);
    void setName(const QString& value);
    void setNameChanged(const QDateTime& value);
    void setDescription(const QString& value);
    void setDescriptionChanged(const QDateTime& value);
    void setDefaultUserName(const QString& value);
    void setDefaultUserNameChanged(const QDateTime& value);
    void setSettingsChanged(const QDateTime& value);
    void setMaintenanceHistoryDays(int value);
    void setColor(const QColor& value);
    void setProtectTitle(bool value);
    void setProtectUsername(bool value);
    void setProtectPassword(bool value);
    void setProtectUrl(bool value);
    void setProtectNotes(bool value);
    void addCustomIcon(const QUuid& uuid, const QImage& icon);
    void addCustomIconScaled(const QUuid& uuid, const QImage& icon);
    void removeCustomIcon(const QUuid& uuid);
    void copyCustomIcons(const QSet<QUuid>& iconList, const Metadata* otherMetadata);
    QUuid findCustomIcon(const QImage& candidate);
    void setRecycleBinEnabled(bool value);
    void setRecycleBin(Group* group);
    void setRecycleBinChanged(const QDateTime& value);
    void setEntryTemplatesGroup(Group* group);
    void setEntryTemplatesGroupChanged(const QDateTime& value);
    void setLastSelectedGroup(Group* group);
    void setLastTopVisibleGroup(Group* group);
    void setMasterKeyChanged(const QDateTime& value);
    void setMasterKeyChangeRec(int value);
    void setMasterKeyChangeForce(int value);
    void setHistoryMaxItems(int value);
    void setHistoryMaxSize(int value);
    void setUpdateDatetime(bool value);
    /*
     * Copy all attributes from other except:
     * - Group pointers/uuids
     * - Master key changed date
     * - Custom icons
     * - Custom fields
     * - Settings changed date
     */
    void copyAttributesFrom(const Metadata* other);

signals:
    void nameTextChanged();
    void modified();

private:
    template <class P, class V> bool set(P& property, const V& value);
    template <class P, class V> bool set(P& property, const V& value, QDateTime& dateTime);

    QByteArray hashImage(const QImage& image);

    MetadataData m_data;

    QHash<QUuid, QImage> m_customIcons;
    mutable QHash<QUuid, QPixmapCache::Key> m_customIconCacheKeys;
    mutable QHash<QUuid, QPixmapCache::Key> m_customIconScaledCacheKeys;
    QList<QUuid> m_customIconsOrder;
    QHash<QByteArray, QUuid> m_customIconsHashes;

    QPointer<Group> m_recycleBin;
    QDateTime m_recycleBinChanged;
    QPointer<Group> m_entryTemplatesGroup;
    QDateTime m_entryTemplatesGroupChanged;
    QPointer<Group> m_lastSelectedGroup;
    QPointer<Group> m_lastTopVisibleGroup;

    QDateTime m_masterKeyChanged;
    QDateTime m_settingsChanged;

    QPointer<CustomData> m_customData;

    bool m_updateDatetime;
};

#endif // KEEPASSX_METADATA_H
