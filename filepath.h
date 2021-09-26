#ifndef FILEPATH_H
#define FILEPATH_H

#include <QDialog>
#include <QDir>
#include <QFileSystemModel>
#include <QTreeView>
#include <QMessageBox>

namespace Ui {
class FilePath;
}

class FilePath : public QDialog {
  Q_OBJECT

 public:
  explicit FilePath(QWidget* parent = nullptr);
  ~FilePath();

  const QModelIndex& getSelectedIndex() const;

  const QModelIndex& desitantionIndex() const;

 private slots:
  void on_buttonBox_rejected();
  void on_treeView_activated(const QModelIndex& index);
  void on_buttonBox_accepted();

 private:
  Ui::FilePath* ui;
  QModelIndex m_desitantionIndex;
  QFileSystemModel* m_fsModel;

  void init();
};

#endif // FILEPATH_H
