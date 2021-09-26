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
  setWindowTitle("File Manager");

  m_msg = UserMessages::instance(this);
}

QSharedPointer<QModelIndex> MainWindow::getCurrentIndexForSelectedDir()
{
  QModelIndex currentIndex = ui->listView->currentIndex();
  QString isNotDirError = m_msg->readData("Error_Is_Not_Dir").toString();
  QString noSelectedError = m_msg->readData("Error_Nothing_Selected").toString();
  QString errorTitle = m_msg->readData("Error_Title").toString();

  if (!m_fsModel->isDir(currentIndex))
  {
    QMessageBox::information(this, errorTitle, isNotDirError);
    return nullptr;
  }

  if (!currentIndex.isValid())
  {
    QMessageBox::information(this, errorTitle, noSelectedError);
    return nullptr;
  }


  return QSharedPointer<QModelIndex>(new QModelIndex(ui->listView->currentIndex()));
}

QSharedPointer<QModelIndex> MainWindow::getCurrentIndexForAnySelected()
{
  QModelIndex currentIndex = ui->listView->currentIndex();
  QString errorTitle = m_msg->readData("Error_Title").toString();
  QString noSelectedError = m_msg->readData("Error_Nothing_Selected").toString();

  if (!currentIndex.isValid())
  {
    QMessageBox::information(this, errorTitle, noSelectedError);
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
  QString inputDialogTitle = m_msg->readData("Input_Dialog_Title").toString();
  QString inputDialogFiledName = m_msg->readData("Input_Dialog_Give_A_Name").toString();
  QString errorTitle = m_msg->readData("Error_Title").toString();
  QString errorGiveName = m_msg->readData("Error_No_Name").toString();
  QString dirName = QInputDialog::getText(this, inputDialogTitle,
                                          inputDialogFiledName, QLineEdit::Normal,
                                          QDir::home().dirName(), &ok);

  if (!ok || dirName.isEmpty())
  {
    QMessageBox::information(this, errorTitle, errorGiveName);
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
  QString infoTitle = m_msg->readData("Info_Title").toString();
  QString errorTitle = m_msg->readData("Error_Title").toString();
  QString inputDialogCancel = m_msg->readData("Input_Dialog_Cancel").toString();
  QString errorCouldNotMove = m_msg->readData("Error_Could_Not_Move").toString();

  if (sourceIndex == nullptr)
    return;

  QString srcAbsPath = m_fsModel->filePath(*sourceIndex.data());

  // open dialog for selecting destination
  FilePath* dlg = new FilePath(this);
  dlg->exec();

  QModelIndex desitantionIndex = dlg->desitantionIndex();

  if (!desitantionIndex.isValid())
  {
    QMessageBox::information(this, infoTitle, inputDialogCancel);
    return;
  }

  QString dstPath = m_fsModel->filePath(desitantionIndex);
  QString dstFileName = QString(sourceIndex.data()->data().toString());

  // move file rename fuction uses unix move
  bool ok = m_dir.rename(srcAbsPath,
                         dstPath + QDir::separator() + dstFileName);

  if (!ok)
  {
    QMessageBox::critical(this, errorTitle, errorCouldNotMove);
    return;
  }

  sourceIndex.clear();
}


void MainWindow::on_actionCopy_File_triggered()
{
  QSharedPointer<QModelIndex> sourceIndex = getCurrentIndexForAnySelected();
  QString infoTitle = m_msg->readData("Info_Title").toString();
  QString errorTitle = m_msg->readData("Error_Title").toString();
  QString inputDialogCancel = m_msg->readData("Input_Dialog_Cancel").toString();
  QString errorCouldNotCopy = m_msg->readData("Error_Could_Not_Copy").toString();

  if (sourceIndex == nullptr)
    return;

  QString srcAbsPath = m_fsModel->filePath(*sourceIndex.data());
  // open dialog for selecting destination
  FilePath* dlg = new FilePath(this);
  dlg->exec();

  QModelIndex desitantionIndex = dlg->desitantionIndex();

  if (!desitantionIndex.isValid())
  {
    QMessageBox::information(this, infoTitle, inputDialogCancel);
    return;
  }

  QString dstPath = m_fsModel->filePath(desitantionIndex);
  QString dstFileName = QString(sourceIndex.data()->data().toString());

  // copy file
  bool ok = QFile::copy(srcAbsPath,
                        dstPath + QDir::separator() + dstFileName);

  if (!ok)
  {
    QMessageBox::critical(this, errorTitle, errorCouldNotCopy);
    return;
  }

  sourceIndex.clear();
}


void MainWindow::on_actionDelete_File_triggered()
{
  QSharedPointer<QModelIndex> sourceIndex = getCurrentIndexForAnySelected();
  QString errorTitle = m_msg->readData("Error_Title").toString();
  QString errorCouldNotRemove = m_msg->readData("Error_Could_Not_Remove").toString();

  if (sourceIndex == nullptr)
    return;

  QString srcAbsPath = m_fsModel->filePath(*sourceIndex.data());

  bool ok = QFile::remove(srcAbsPath);

  if (!ok)
  {
    QMessageBox::critical(this, errorTitle, errorCouldNotRemove);
    return;
  }

  sourceIndex.clear();
}

