#include "filepath.h"
#include "ui_filepath.h"

FilePath::FilePath(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::FilePath)
{
  ui->setupUi(this);
  init();
}

FilePath::~FilePath()
{
  delete ui;
}

void FilePath::init()
{
  m_fsModel = new QFileSystemModel(this);
  m_fsModel->setRootPath(QDir::currentPath());
  ui->treeView->setModel(m_fsModel);

  for (int i = 1; i < m_fsModel->columnCount(); ++i )\
  {
    ui->treeView->hideColumn(i);
  }
}


void FilePath::on_buttonBox_rejected()
{
  reject();
}


void FilePath::on_treeView_activated(const QModelIndex& index)
{
  ui->treeView->expand(index);
}


void FilePath::on_buttonBox_accepted()
{
  m_desitantionIndex = ui->treeView->currentIndex();
}


const QModelIndex& FilePath::desitantionIndex() const
{
  return m_desitantionIndex;
}

