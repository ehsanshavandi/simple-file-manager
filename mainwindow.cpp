#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  init();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::init()
{
  m_fsModel = new QFileSystemModel(this);
  m_fsModel->setRootPath(QDir::currentPath());
  ui->treeView->setModel(m_fsModel);
  ui->listView->setModel(m_fsModel);
  ui->listView->setViewMode(QListView::ViewMode::ListMode);
  this->setWindowTitle("File Manager");
}

QSharedPointer<QModelIndex> MainWindow::getCurrentIndexForSelectedDir()
{
  QModelIndex currentIndex = ui->listView->currentIndex();

  if (!m_fsModel->isDir(currentIndex))
  {
    QMessageBox::information(this, "Error", "selected item is not a directory");
    return nullptr;
  }

  if (!currentIndex.isValid())
  {
    QMessageBox::information(this, "Error", "No selected item");
    return nullptr;
  }


  return QSharedPointer<QModelIndex>(new QModelIndex(ui->listView->currentIndex()));
}

QSharedPointer<QModelIndex> MainWindow::getCurrentIndexForAnySelected()
{
  QModelIndex currentIndex = ui->listView->currentIndex();

  if (!currentIndex.isValid())
  {
    QMessageBox::information(this, "Error", "No selected item");
    return nullptr;
  }


  return QSharedPointer<QModelIndex>(new QModelIndex(ui->listView->currentIndex()));
}

void MainWindow::on_treeView_activated(const QModelIndex& index)
{
  ui->listView->setRootIndex(index);
}


void MainWindow::on_listView_activated(const QModelIndex& index)
{
  ui->listView->setRootIndex(index);
  ui->treeView->expand(index);
}



void MainWindow::on_actionNew_Directory_triggered()
{
  bool ok;
  QString dirName = QInputDialog::getText(this, tr("Fill the box"),
                                          tr("User name:"), QLineEdit::Normal,
                                          QDir::home().dirName(), &ok);

  if (!ok || dirName.isEmpty())
  {
    QMessageBox::information(this, "Error", "Give a name for wanted directory");
    return;
  }

  QSharedPointer<QModelIndex> currentIndex = getCurrentIndexForSelectedDir();

  if (currentIndex == nullptr)
    return;

  m_fsModel->mkdir(*currentIndex.data(), dirName);
  currentIndex.clear();
}


void MainWindow::on_actionRemove_Directory_triggered()
{

  QSharedPointer<QModelIndex> currentIndex = getCurrentIndexForSelectedDir();

  if (currentIndex == nullptr)
    return;

  m_fsModel->rmdir(*currentIndex.data());
  currentIndex.clear();
}


void MainWindow::on_actionback_triggered()
{
  QSharedPointer<QModelIndex> currentIndex = getCurrentIndexForAnySelected();

  if (currentIndex == nullptr)
    return;

  ui->listView->setRootIndex(currentIndex.data()->parent());
  currentIndex.clear();
}


void MainWindow::on_actionforward_triggered()
{
  QSharedPointer<QModelIndex> currentIndex = getCurrentIndexForAnySelected();

  if (currentIndex == nullptr)
    return;

  ui->listView->setRootIndex(*currentIndex.data());
  currentIndex.clear();
}


void MainWindow::on_actionMove_File_triggered()
{
  QSharedPointer<QModelIndex> sourceIndex = getCurrentIndexForAnySelected();

  if (sourceIndex == nullptr)
    return;

  QString srcAbsPath = m_fsModel->filePath(*sourceIndex.data());

  // open dialog for selecting destination
  FilePath* dlg = new FilePath(this);
  dlg->exec();

  QModelIndex desitantionIndex = dlg->desitantionIndex();

  if (!desitantionIndex.isValid())
  {
    QMessageBox::information(this, "Info", "Canceled the job");
    return;
  }

  QString dstPath = m_fsModel->filePath(desitantionIndex);
  QString dstFileName = QString(sourceIndex.data()->data().toString());

  // move file rename fuction uses unix move
  bool ok = m_dir.rename(srcAbsPath,
                         dstPath + QDir::separator() + dstFileName);

  if (!ok)
  {
    QMessageBox::critical(this, "Error", "Could not perform the action");
    return;
  }

  sourceIndex.clear();
}


void MainWindow::on_actionCopy_File_triggered()
{
  QSharedPointer<QModelIndex> sourceIndex = getCurrentIndexForAnySelected();

  if (sourceIndex == nullptr)
    return;

  QString srcAbsPath = m_fsModel->filePath(*sourceIndex.data());
  // open dialog for selecting destination
  FilePath* dlg = new FilePath(this);
  dlg->exec();

  QModelIndex desitantionIndex = dlg->desitantionIndex();

  if (!desitantionIndex.isValid())
  {
    QMessageBox::information(this, "Info", "Canceled the job");
    return;
  }

  QString dstPath = m_fsModel->filePath(desitantionIndex);
  QString dstFileName = QString(sourceIndex.data()->data().toString());

  // copy file
  bool ok = QFile::copy(srcAbsPath,
                        dstPath + QDir::separator() + dstFileName);

  if (!ok)
  {
    QMessageBox::critical(this, "Error", "Could not perform the action");
    return;
  }

  sourceIndex.clear();
}


void MainWindow::on_actionDelete_File_triggered()
{
  QSharedPointer<QModelIndex> sourceIndex = getCurrentIndexForAnySelected();

  if (sourceIndex == nullptr)
    return;

  QString srcAbsPath = m_fsModel->filePath(*sourceIndex.data());

  bool ok = QFile::remove(srcAbsPath);

  if (!ok)
  {
    QMessageBox::critical(this, "Error", "Could not perform the action");
    return;
  }

  sourceIndex.clear();
}

