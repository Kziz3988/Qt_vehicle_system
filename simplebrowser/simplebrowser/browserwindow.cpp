#include "MainWindow.h"
#include "browser.h"
#include "browserwindow.h"
#include "tabwidget.h"
#include "urllineedit.h"
#include "webview.h"
#include <QApplication>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QProgressBar>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWebEngineView>



BrowserWindow::BrowserWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
    , m_tabWidget(new TabWidget(this))
    , m_progressBar(new QProgressBar(this))
    , m_historyBackAction(nullptr)
    , m_historyForwardAction(nullptr)
    , m_stopAction(nullptr)
    , m_reloadAction(nullptr)
    , m_stopReloadAction(nullptr)
    , m_urlLineEdit(new UrlLineEdit(this))   
    , m_backButton(new QPushButton(tr("Back"), this))

{
    setToolButtonStyle(Qt::ToolButtonFollowStyle);

    setAttribute(Qt::WA_DeleteOnClose, true);


    QToolBar *toolbar = createToolBar();
    addToolBar(toolbar);


    //添加菜单栏
    menuBar()->addMenu(createFileMenu(m_tabWidget));
    menuBar()->addMenu(createViewMenu(toolbar));

    //实例化一个widget
    QWidget *centralWidget = new QWidget(this);

    //进行布局设置,先实例化一个QVBoxLayout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);//表示各个控件之间的上下间距
    layout->setMargin(0);//表示控件与窗体的左右边距
    addToolBarBreak();

    m_progressBar->setMaximumHeight(1);
    m_progressBar->setTextVisible(false);
    m_progressBar->setStyleSheet(QStringLiteral("QProgressBar {border: 0px } QProgressBar::chunk { background-color: red; }"));
    layout->addWidget(m_progressBar);

    // Add back button
    m_backButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(m_backButton, &QPushButton::clicked, this, [this]() {

    });
    layout->addWidget(m_backButton);

    connect(m_backButton, SIGNAL(clicked(bool)), this, SLOT(onBackButtonClicked()));

    //添加一个widget,将tab页放进去
    layout->addWidget(m_tabWidget);

    //将上一句的布局放到，上面实例化的QWidget中
    centralWidget->setLayout(layout);

    setCentralWidget(centralWidget);

    
    connect(m_tabWidget, &TabWidget::titleChanged, this, &BrowserWindow::handleWebViewTitleChanged);
    //鼠标移动到连接上面statusBar部分会显示相应连接内容
    connect(m_tabWidget, &TabWidget::linkHovered, [this](const QString& url) {
        statusBar()->showMessage(url);
    });
    //加载页面的时候，进度条进行读取
    connect(m_tabWidget, &TabWidget::loadProgress, this, &BrowserWindow::handleWebViewLoadProgress);
    //url改变的时候，输入框的URL就要进行修改
    connect(m_tabWidget, &TabWidget::urlChanged, this, &BrowserWindow::handleWebViewUrlChanged);
    //icon图改变的时候，输入框的ico图就要进行修改
    connect(m_tabWidget, &TabWidget::iconChanged, this, &BrowserWindow::handleWebViewIconChanged);
    //
    connect(m_tabWidget, &TabWidget::webActionEnabledChanged, this, &BrowserWindow::handleWebActionEnabledChanged);
    //输入框输入地址，按回车进入相应页面，并且在输入框显示默认图标
    connect(m_urlLineEdit, &QLineEdit::returnPressed, this, [this]() {
        m_urlLineEdit->setFavIcon(QIcon(QStringLiteral(":defaulticon.png")));
        loadPage(m_urlLineEdit->url());
    });
    //刚打开浏览器加载页面的时候，url输入框显示默认icon图
    m_urlLineEdit->setFavIcon(QIcon(QStringLiteral(":defaulticon.png")));
    //初始化的时候设置窗口title
    handleWebViewTitleChanged(tr("Qt Simple Browser"));
    //创建一个初始的tab页面
    m_tabWidget->createTab();
}

BrowserWindow::~BrowserWindow()
{
}


QSize BrowserWindow::sizeHint() const
{
    QRect desktopRect = QApplication::desktop()->screenGeometry();
    QSize size = desktopRect.size() * qreal(0.9);
    return size;
}

//菜单栏第一个菜单File
QMenu *BrowserWindow::createFileMenu(TabWidget *tabWidget)
{
    //设置显示的名称File
    QMenu *fileMenu = new QMenu(tr("&File"));
    //添加子菜单New Window，调用创建新窗口handleNewWindowTriggered这个函数，并且设置快捷键
    fileMenu->addAction(tr("&New Window"), this, &BrowserWindow::handleNewWindowTriggered, QKeySequence::New);

    QAction *newTabAction = new QAction(QIcon(QLatin1String(":addtab.png")), tr("New &Tab"), this);
    newTabAction->setShortcuts(QKeySequence::AddTab);
    newTabAction->setIconVisibleInMenu(false);
    connect(newTabAction, &QAction::triggered, tabWidget, &TabWidget::createTab);
    fileMenu->addAction(newTabAction);

    //添加子菜单Open File，调用打开文件handleFileOpenTriggered函数，设置快捷键
    fileMenu->addAction(tr("&Open File..."), this, &BrowserWindow::handleFileOpenTriggered, QKeySequence::Open);

    //添加一条横线分隔
    fileMenu->addSeparator();

    //添加子菜单关闭当前tab(各句含义参考上面)
    QAction *closeTabAction = new QAction(QIcon(QLatin1String(":closetab.png")), tr("&Close Tab"), this);
    closeTabAction->setShortcuts(QKeySequence::Close);
    closeTabAction->setIconVisibleInMenu(false);
    connect(closeTabAction, &QAction::triggered, [tabWidget]() {
        tabWidget->closeTab(tabWidget->currentIndex());
    });
    fileMenu->addAction(closeTabAction);

    //添加子菜单退出(各句含义参考上面)
    QAction *closeAction = new QAction(tr("&Quit"),this);
    closeAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
    connect(closeAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(closeAction);

    connect(fileMenu, &QMenu::aboutToShow, [closeAction]() {
        if (Browser::instance().windows().count() == 1)
            closeAction->setText(tr("&Quit"));
        else
            closeAction->setText(tr("&Close Window"));
    });
    //返回创建好的第一个菜单
    return fileMenu;
}

QMenu *BrowserWindow::createViewMenu(QToolBar *toolbar)
{
    //新建第二个大菜单
    QMenu *viewMenu = new QMenu(tr("&View"));

    m_stopAction = viewMenu->addAction(tr("&Stop"));

    QList<QKeySequence> shortcuts;
    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_Period));
    shortcuts.append(Qt::Key_Escape);
    m_stopAction->setShortcuts(shortcuts);
    connect(m_stopAction, &QAction::triggered, [this]() {
        m_tabWidget->triggerWebPageAction(QWebEnginePage::Stop);
    });

    //添加第二个子菜单，重载页面
    m_reloadAction = viewMenu->addAction(tr("Reload Page"));
    m_reloadAction->setShortcuts(QKeySequence::Refresh);
    connect(m_reloadAction, &QAction::triggered, [this]() {
        m_tabWidget->triggerWebPageAction(QWebEnginePage::Reload);
    });


    QAction *zoomIn = viewMenu->addAction(tr("Zoom &In"));
    zoomIn->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Plus));
    connect(zoomIn, &QAction::triggered, [this]() {
        if (currentTab())
            currentTab()->setZoomFactor(currentTab()->zoomFactor() + 0.1);
    });

    //设置页面的百分比(缩小)
    QAction *zoomOut = viewMenu->addAction(tr("Zoom &Out"));
    zoomOut->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Minus));
    connect(zoomOut, &QAction::triggered, [this]() {
        if (currentTab())
            currentTab()->setZoomFactor(currentTab()->zoomFactor() - 0.1);
    });

    //设置页面的百分比(设置成默认的100%)
    QAction *resetZoom = viewMenu->addAction(tr("Reset &Zoom"));
    resetZoom->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_0));
    connect(resetZoom, &QAction::triggered, [this]() {
        if (currentTab())
            currentTab()->setZoomFactor(1.0);
    });

    //加一条横线
    viewMenu->addSeparator();

    //隐藏/显示ToolBar
    QAction *viewToolbarAction = new QAction(tr("Hide Toolbar"),this);
    viewToolbarAction->setShortcut(tr("Ctrl+|"));
    connect(viewToolbarAction, &QAction::triggered, [toolbar,viewToolbarAction]() {
        if (toolbar->isVisible()) {
            viewToolbarAction->setText(tr("Show Toolbar"));
            toolbar->close();
        } else {
            viewToolbarAction->setText(tr("Hide Toolbar"));
            toolbar->show();
        }
    });
    viewMenu->addAction(viewToolbarAction);

    QAction *viewStatusbarAction = new QAction(tr("Hide Status Bar"), this);
    viewStatusbarAction->setShortcut(tr("Ctrl+/"));
    connect(viewStatusbarAction, &QAction::triggered, [this, viewStatusbarAction]() {
        if (statusBar()->isVisible()) {
            viewStatusbarAction->setText(tr("Show Status Bar"));
            statusBar()->close();
        } else {
            viewStatusbarAction->setText(tr("Hide Status Bar"));
            statusBar()->show();
        }
    });
    viewMenu->addAction(viewStatusbarAction);

    //返回第二个大菜单
    return viewMenu;
}

QMenu *BrowserWindow::createWindowMenu(TabWidget *tabWidget)
{
    QMenu *menu = new QMenu(tr("&Window"));

    QAction *nextTabAction = new QAction(tr("Show Next Tab"), this);
    QList<QKeySequence> shortcuts;
    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_BraceRight));
    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_PageDown));
    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_BracketRight));
    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_Less));
    nextTabAction->setShortcuts(shortcuts);
    connect(nextTabAction, &QAction::triggered, tabWidget, &TabWidget::nextTab);

    QAction *previousTabAction = new QAction(tr("Show Previous Tab"), this);
    shortcuts.clear();
    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_BraceLeft));
    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_PageUp));
    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_BracketLeft));
    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_Greater));
    previousTabAction->setShortcuts(shortcuts);
    connect(previousTabAction, &QAction::triggered, tabWidget, &TabWidget::previousTab);

    connect(menu, &QMenu::aboutToShow, [this, menu, nextTabAction, previousTabAction]() {
        menu->clear();
        menu->addAction(nextTabAction);
        menu->addAction(previousTabAction);
        menu->addSeparator();

        QVector<BrowserWindow*> windows = Browser::instance().windows();
        int index(-1);
        for (auto window : windows) {
            QAction *action = menu->addAction(window->windowTitle(), this, &BrowserWindow::handleShowWindowTriggered);
            action->setData(++index);
            action->setCheckable(true);
            if (window == this)
                action->setChecked(true);
        }
    });
    return menu;
}

QMenu *BrowserWindow::createHelpMenu()
{
    QMenu *helpMenu = new QMenu(tr("&Help"));
    helpMenu->addAction(tr("About &Qt"), qApp, QApplication::aboutQt);
    return helpMenu;
}


QToolBar *BrowserWindow::createToolBar()
{

    QToolBar *navigationBar = new QToolBar(tr("工具栏"));

    navigationBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    //设置工具栏是未选中状态
    navigationBar->toggleViewAction()->setEnabled(false);

    //添加工具栏返回上一页事件
    m_historyBackAction = new QAction(this);
    //设置返回快捷键
    m_historyBackAction->setShortcuts(QKeySequence::Back);

    m_historyBackAction->setIconVisibleInMenu(false);
    //设置该行为的图标
    m_historyBackAction->setIcon(QIcon(QStringLiteral(":go-previous.png")));

    connect(m_historyBackAction, &QAction::triggered, [this]() {
        m_tabWidget->triggerWebPageAction(QWebEnginePage::Back);
    });
    //将设置好的back行为添加到新建的工具栏属性中
    navigationBar->addAction(m_historyBackAction);

    //添加工具栏下一页事件(参考上一页事件)
    m_historyForwardAction = new QAction(this);
    m_historyForwardAction->setShortcuts(QKeySequence::Forward);
    m_historyForwardAction->setIconVisibleInMenu(false);
    m_historyForwardAction->setIcon(QIcon(QStringLiteral(":go-next.png")));
    connect(m_historyForwardAction, &QAction::triggered, [this]() {
        m_tabWidget->triggerWebPageAction(QWebEnginePage::Forward);
    });
    navigationBar->addAction(m_historyForwardAction);

    m_stopReloadAction = new QAction(this);
    connect(m_stopReloadAction, &QAction::triggered, [this]() {
        m_tabWidget->triggerWebPageAction(QWebEnginePage::WebAction(m_stopReloadAction->data().toInt()));
    });
    navigationBar->addAction(m_stopReloadAction);

    navigationBar->addWidget(m_urlLineEdit);
    int size = m_urlLineEdit->sizeHint().height();

    //设置工具栏的Icon图的尺寸的宽和高，都是搜索框的高这个值
    navigationBar->setIconSize(QSize(size, size));

    //返回一个ToolBar工具栏
    return navigationBar;
}

void BrowserWindow::handleWebViewIconChanged(const QIcon &icon)
{
    m_urlLineEdit->setFavIcon(icon);
}

void BrowserWindow::handleWebViewUrlChanged(const QUrl &url)
{
    m_urlLineEdit->setUrl(url);
    if (url.isEmpty())
        m_urlLineEdit->setFocus();
}

void BrowserWindow::handleWebActionEnabledChanged(QWebEnginePage::WebAction action, bool enabled)
{
    switch (action) {
    case QWebEnginePage::Back:
        m_historyBackAction->setEnabled(enabled);
        break;
    case QWebEnginePage::Forward:
        m_historyForwardAction->setEnabled(enabled);
        break;
    case QWebEnginePage::Reload:
        m_reloadAction->setEnabled(enabled);
        break;
    case QWebEnginePage::Stop:
        m_stopAction->setEnabled(enabled);
        break;
    default:
        qWarning("Unhandled webActionChanged singal");
    }
}

void BrowserWindow::handleWebViewTitleChanged(const QString &title)
{
    if (title.isEmpty())
        setWindowTitle(tr("QT制作的车载浏览器"));
    else
        setWindowTitle(tr("%1 - QT制作的车载浏览器").arg(title));
}

void BrowserWindow::handleNewWindowTriggered()
{
    BrowserWindow *window = new BrowserWindow();
    Browser::instance().addWindow(window);
    window->loadHomePage();
}




void BrowserWindow::handleFileOpenTriggered()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open Web Resource"), QString(),
                                                tr("Web Resources (*.html *.htm *.svg *.png *.gif *.svgz);;All files (*.*)"));
    if (file.isEmpty())
        return;
    loadPage(file);
}


void BrowserWindow::closeEvent(QCloseEvent *event)
{
    if (m_tabWidget->count() > 1) {
        int ret = QMessageBox::warning(this, tr("Confirm close"),
                                       tr("你确定你要关闭窗口 ?\n"
                                          "请注意，还有 %1 个窗口开着！").arg(m_tabWidget->count()),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (ret == QMessageBox::No){
            event->ignore();
            return;
        }
    }
    event->accept();
    deleteLater();
}

//默认首页
void BrowserWindow::loadHomePage()
{
    loadPage(QStringLiteral("http://www.bing.com"));
}

void BrowserWindow::loadPage(const QString &page)
{
    loadPage(QUrl::fromUserInput(page));
}

//进来的是个url
void BrowserWindow::loadPage(const QUrl &url)
{
    //判断进来的url是否可用
    if (url.isValid()) {
        m_urlLineEdit->setUrl(url);
        //打开网页
        m_tabWidget->setUrl(url);
    }
}

TabWidget *BrowserWindow::tabWidget() const
{
    return m_tabWidget;
}

WebView *BrowserWindow::currentTab() const
{
    return m_tabWidget->currentWebView();
}

//加载页面时，进度条的行为
void BrowserWindow::handleWebViewLoadProgress(int progress)
{
    static QIcon stopIcon(QStringLiteral(":process-stop.png"));
    static QIcon reloadIcon(QStringLiteral(":view-refresh.png"));

    if (progress < 100 && progress > 0) {
        m_stopReloadAction->setData(QWebEnginePage::Stop);
        m_stopReloadAction->setIcon(stopIcon);
        m_stopReloadAction->setToolTip(tr("Stop loading the current page"));
    } else {
        m_stopReloadAction->setData(QWebEnginePage::Reload);
        m_stopReloadAction->setIcon(reloadIcon);
        m_stopReloadAction->setToolTip(tr("Reload the current page"));
    }
    m_progressBar->setValue(progress < 100 ? progress : 0);
}

//
void BrowserWindow::handleShowWindowTriggered()
{
    if (QAction *action = qobject_cast<QAction*>(sender())) {
        int offset = action->data().toInt();
        QVector<BrowserWindow*> windows = Browser::instance().windows();
        windows.at(offset)->activateWindow();
        windows.at(offset)->currentTab()->setFocus();
    }
}
//
void BrowserWindow::onBackButtonClicked() {
    // 实现返回按钮的功能
    emit JumpToMainMenu();
    this->hide();
}

void BrowserWindow::ShowExplorerSlot()
{
    this->show();
}
