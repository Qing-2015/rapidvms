#include "live/vscvidlive.h"
#include "live/vscview.h"
#include "vscvwidget.h"
#include "vscvideowall.h"
#include "vscloading.hpp"
#include "common/vidtree/vscvidtreecam.h"
#include <QDesktopWidget>


Q_DECLARE_METATYPE(QDockWidget::DockWidgetFeatures)

VSCVidLive::VSCVidLive(ClientFactory &pFactory, QTabWidget &pTab, QMainWindow *parent)
: VSCVidInf(pFactory, pTab, parent)
{
	
#if 1	
	m_pView = new VSCView(m_pFactory, &m_pMainArea, m_pMainArea, tr("Main View"));
	m_pView->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint  );
	//connect(m_pDeviceList, SIGNAL(CameraDoubleClicked(int)), m_pView, SLOT(CameraDoubleClicked(int)));

	m_pMainArea.addTab(m_pView,QIcon(tr(":/view/resources/3x3.png")), tr("Main View"));
	//connect(m_pEventThread, SIGNAL(EventNotify(int, int)), 
		//	m_pView, SLOT(DeviceEvent(int, int)));
#endif
	connect(&m_pMainArea, SIGNAL(tabCloseRequested(int)), this, SLOT(MainCloseTab(int)));

	m_pCameraTree = new VSCVidTreeCam(m_pFactory, parent);
	m_pCameraTree->Init();
	m_pGroupTree = new VSCVidTreeInf(m_pFactory, parent);
	m_pGroupTree->Init();
	m_pViewTree = new VSCVidTreeInf(m_pFactory, parent);
	m_pViewTree->Init();
	m_pEmapTree = new VSCVidTreeInf(m_pFactory, parent);
	m_pEmapTree->Init();

	connect(m_pCameraTree, SIGNAL(CameraSelected(std::string, std::string, std::string)), 
		m_pView, SLOT(CameraDoubleClicked(std::string, std::string, std::string)));


#if 0
	QTreeWidgetItem * pGroup = new  QTreeWidgetItem(m_pGroupTree);
	pGroup->setText(0, QApplication::translate("", "Group Tree", 0));

	m_pGroupTree->addTopLevelItem(pGroup);
#endif
	
}
VSCVidLive::~VSCVidLive()
{
	
}

void VSCVidLive::MainCloseTab(int index)
{
    QWidget *wdgt = m_pMainArea.widget(index);
    m_pMainArea.setCurrentWidget(wdgt);
    /* Frist view can not be closed */
    if (wdgt == m_pView)
    {
        return;
    }
#if 0
    if (wdgt == m_pEMap)	
    {
		m_pMainArea->removeTab(index);
		m_pEMap->hide();
		return;
    }

    if (wdgt == m_pPanel)	
    {
		m_pMainArea->removeTab(index);
		m_pPanel->hide();
		return;
    }
#endif
    m_pMainArea.removeTab(index);
    if (wdgt)
    {
#if 0
		VSCLoading *loading = new VSCLoading(NULL);
		loading->setGeometry(width()/2, height()/2, 64, 64);
		QCoreApplication::processEvents();
#endif
		delete wdgt;
		wdgt = NULL;
		//delete loading;
    }
}

void VSCVidLive::VidShow()
{
	//m_parent->addDockWidget(Qt::LeftDockWidgetArea, m_pDockDevicelist);

}
void VSCVidLive::VidHide()
{
	//m_parent->removeDockWidget(m_pDockDevicelist);

}

void VSCVidLive::VidNewLiveView()
{
	int tabNum = m_pMainArea.count();
	int currentNum = -1;
	for (int i=0; i<tabNum; i++)
	{
		QWidget *qwidget = m_pMainArea.widget(i);
		VSCView *pView = dynamic_cast<VSCView* >(qwidget);
		if (pView)
		{
			currentNum++;
		}
	}

	VSCLoading *loading = new VSCLoading(NULL);
	loading->show();
	QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);
	loading->setGeometry(rect.width()/2, rect.height()/2, 64, 64);
	QCoreApplication::processEvents();
	
	VSCView *pView = new VSCView(m_pFactory, &m_pMainArea, m_pMainArea, tr("View %1").arg(currentNum+1));
	pView->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint  );
	//connect(m_pDeviceList, SIGNAL(CameraDoubleClicked(int)), m_pView, SLOT(CameraDoubleClicked(int)));

	m_pMainArea.addTab(pView,QIcon(tr(":/view/resources/3x3.png")), tr("View %1").arg(currentNum+1));
	delete loading;
}

void VSCVidLive::VidNewEmap()
{
}

VSCVidTreeInf *VSCVidLive::GetCameraTree()
{
	return m_pCameraTree;
}
VSCVidTreeInf *VSCVidLive::GetGroupTree()
{
	return m_pGroupTree;
}
VSCVidTreeInf *VSCVidLive::GetEmapTree()
{
	return m_pEmapTree;
}
VSCVidTreeInf *VSCVidLive::GetViewTree()
{
	return m_pViewTree;
}
