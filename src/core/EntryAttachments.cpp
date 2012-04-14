/*
 *  Copyright (C) 2012 Felix Geyer <debfx@fobos.de>
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

#include "EntryAttachments.h"

EntryAttachments::EntryAttachments(QObject* parent)
    : QObject(parent)
{
}

QList<QString> EntryAttachments::keys() const
{
    return m_attachments.keys();
}

QByteArray EntryAttachments::value(const QString& key) const
{
    return m_attachments.value(key);
}

bool EntryAttachments::isProtected(const QString& key) const
{
    return m_protectedAttachments.contains(key);
}

void EntryAttachments::set(const QString& key, const QByteArray& value, bool protect)
{
    bool emitModified = false;
    bool addAttachment = !m_attachments.contains(key);

    if (addAttachment) {
        Q_EMIT aboutToBeAdded(key);
    }

    if (addAttachment || m_attachments.value(key) != value) {
        m_attachments.insert(key, value);
        emitModified = true;
    }

    if (protect != m_protectedAttachments.contains(key)) {
        if (protect) {
            m_protectedAttachments.insert(key);
        }
        else {
            m_protectedAttachments.remove(key);
        }

        emitModified = true;
    }

    if (addAttachment) {
        Q_EMIT added(key);
    }
    else {
        Q_EMIT keyModified(key);
    }

    if (emitModified) {
        Q_EMIT modified();
    }
}

void EntryAttachments::remove(const QString& key)
{
    if (!m_attachments.contains(key)) {
        Q_ASSERT(false);
        return;
    }

    Q_EMIT aboutToBeRemoved(key);

    m_attachments.remove(key);
    m_protectedAttachments.remove(key);

    Q_EMIT removed(key);
    Q_EMIT modified();
}

void EntryAttachments::copyFrom(const EntryAttachments* other)
{
    if (*this != *other) {
        Q_EMIT aboutToBeReset();

        m_attachments.clear();
        m_protectedAttachments.clear();

        Q_FOREACH (const QString& key, other->keys()) {
            m_attachments.insert(key, other->value(key));
            if (other->isProtected(key)) {
                m_protectedAttachments.insert(key);
            }
        }

        Q_EMIT reset();
        Q_EMIT modified();
    }
}

void EntryAttachments::clear()
{
    Q_EMIT aboutToBeReset();

    m_attachments.clear();
    m_protectedAttachments.clear();

    Q_EMIT reset();
    Q_EMIT modified();
}

bool EntryAttachments::operator!=(const EntryAttachments& other) const
{
    return m_attachments != other.m_attachments || m_protectedAttachments != other.m_protectedAttachments;
}
