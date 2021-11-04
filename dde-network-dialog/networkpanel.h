/*
 * Copyright (C) 2011 ~ 2021 Deepin Technology Co., Ltd.
 *
 * Author:     donghualin <donghualin@uniontech.com>
 *
 * Maintainer: donghualin <donghualin@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NETWORKPANEL_H
#define NETWORKPANEL_H

#include "item/devicestatushandler.h"

#include <NetworkManagerQt/Device>

#include <QWindow>

#include <DListView>
#include <DStyledItemDelegate>

namespace dde {
  namespace network {
    enum class DeviceType;
    class NetworkDeviceBase;
  }
}

class NetItem;
class QStandardItemModel;
class QTimer;
class QScrollArea;


using namespace dde::network;

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

class NetworkPanel : public QObject
{
    Q_OBJECT

public:
    explicit NetworkPanel(QObject *parent = Q_NULLPTR);
    ~NetworkPanel();

    void invokeMenuItem(const QString &menuId);
    bool needShowControlCenter();
    const QString contextMenu() const;
    QWidget *itemApplet();
    bool hasDevice();
    void passwordError(const QString &path);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void setControlBackground();
    void initUi();
    void initConnection();
    void getPluginState();
    void updateView();                                                  // 更新网络列表内容大小
    void updateItems();
    bool deviceEnabled(const DeviceType &deviceType) const;
    void setDeviceEnabled(const DeviceType &deviceType, bool enabeld);

    int getStrongestAp();
    int deviceCount(const DeviceType &devType);
    QStringList ipTipsMessage(const DeviceType &devType);
    void expandPasswordInput();

private Q_SLOTS:
    void onDeviceAdded(QList<NetworkDeviceBase *> devices);
    void onDeviceStatusChanged(NetworkManager::Device::State newstate, NetworkManager::Device::State oldstate, NetworkManager::Device::StateChangeReason reason);
    void onUpdatePlugView();

    void onClickListView(const QModelIndex &index);
    void updateSize();                                                  // 更新窗口大小

private:
    PluginState m_pluginState;

    QTimer *m_switchWireTimer;
    QTimer *m_wirelessScanTimer;

    bool m_switchWire;

    QStandardItemModel *m_model;

    QScrollArea *m_applet;
    QWidget *m_centerWidget;
    DListView *m_netListView;
    // 判断定时的时间是否到,否则不重置计时器
    bool m_timeOut;

    QList<NetItem *> m_items;
    NetItem *m_selectItem;
    // 记录无线设备Path,防止信号重复连接
    QSet<QString> m_wirelessDevicePath;
    QString m_lastActiveWirelessDevicePath;
    QString m_reconnectPath;
};

class NetworkDelegate : public DStyledItemDelegate
{
    Q_OBJECT

public:
    explicit NetworkDelegate(QAbstractItemView *parent = Q_NULLPTR);
    ~NetworkDelegate() Q_DECL_OVERRIDE;

Q_SIGNALS:
    void closeClicked(const QModelIndex &);

protected:
    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    bool needDrawLine(const QModelIndex &index) const;
    bool cantHover(const QModelIndex &index) const;

    void drawCheck(QPainter *painter, QRect &rect, QPen &pen, int radius) const;
    void drawFork(QPainter *painter, QRect &rect, QPen &pen, int radius) const;
    void drawLoading(QPainter *painter, QRect &rect, int diameter) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

    QRect checkRect(const QRect &rct) const;

    QList<QColor> createDefaultIndicatorColorList(QColor color) const;

private:
    QAbstractItemView *m_parentWidget;
    mutable double m_currentDegree;
    QTimer *m_refreshTimer;
    mutable QList<QModelIndex> m_ConnectioningIndexs;
};

#endif // NETWORKPANEL_H