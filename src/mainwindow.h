#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFileSystemModel>
#include <QListView>
#include <QTreeView>
#include <QMessageBox>
#include <QInputDialog>
#include <QSharedPointer>
#include "filepath.h"
#include "src/messages.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:
  void on_treeView_activated(const QModelIndex& index);
  void on_listView_activated(const QModelIndex& index);
  void on_actionNew_Directory_triggered();
  void on_actionRemove_Directory_triggered();
  void on_actionback_triggered();
  void on_actionforward_triggered();
  void on_actionMove_File_triggered();
  void on_actionCopy_File_triggered();
  void on_actionDelete_File_triggered();

 private:
  Ui::MainWindow* ui;

  QDir m_dir;
  QFileSystemModel* m_fsModel;
  UserMessages* m_msg;
  QMap<QString, QString> m_msgMap;

  void init();
  QSharedPointer<QModelIndex> getCurrentIndexForSelectedDir();
  QSharedPointer<QModelIndex> getCurrentIndexForAnySelected();

};
#endif // MAINWINDOW_H
