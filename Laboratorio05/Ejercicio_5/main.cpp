#include <QApplication>
#include <QCheckBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QStringList>
#include <QVector>
#include <QVBoxLayout>
#include <QWidget>

class SynonymRow : public QWidget {
    Q_OBJECT

public:
    explicit SynonymRow(QWidget *parent = nullptr) : QWidget(parent) {
        auto *layout = new QHBoxLayout(this);
        layout->setContentsMargins(2, 2, 2, 2);

        checkBox = new QCheckBox(this);
        checkBox->setChecked(false);
        text = new QLineEdit(QStringLiteral("Subir - Ascender"), this);
        text->setReadOnly(true);
        auto *editButton = new QPushButton(QStringLiteral("Editar"), this);
        auto *removeButton = new QPushButton(QStringLiteral("Quitar"), this);

        layout->addWidget(checkBox);
        layout->addWidget(text, 1);
        layout->addWidget(editButton);
        layout->addWidget(removeButton);

        connect(editButton, &QPushButton::clicked, this,
                [this]() { emit editRequested(this); });
        connect(removeButton, &QPushButton::clicked, this,
                [this]() { emit removeRequested(this); });
    }

    QString synonyms() const { return text->text(); }
    void setSynonyms(const QString &value) { text->setText(value); }
    void markDuplicate(bool duplicate) {
        text->setStyleSheet(duplicate ? QStringLiteral("QLineEdit { background: #ffe0e0; }")
                                      : QString());
        text->setToolTip(duplicate ? QStringLiteral("Este par de sinonimos esta duplicado")
                                   : QString());
    }

signals:
    void editRequested(SynonymRow *row);
    void removeRequested(SynonymRow *row);

private:
    QCheckBox *checkBox;
    QLineEdit *text;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle(QStringLiteral("Checklist de sinonimos"));
        resize(670, 370);

        auto *central = new QWidget(this);
        auto *root = new QVBoxLayout(central);
        auto *header = new QHBoxLayout();

        statusLabel = new QLabel(QStringLiteral("Checklist de sinonimos"), central);
        auto *addButton = new QPushButton(QStringLiteral("Agregar sinónimos"), central);
        header->addWidget(statusLabel, 1);
        header->addWidget(addButton);
        root->addLayout(header);

        auto *scroll = new QScrollArea(central);
        scroll->setWidgetResizable(true);
        auto *container = new QWidget(scroll);
        tasksLayout = new QVBoxLayout(container);
        tasksLayout->setAlignment(Qt::AlignTop);
        scroll->setWidget(container);
        root->addWidget(scroll, 1);
        setCentralWidget(central);

        connect(addButton, &QPushButton::clicked, this, &MainWindow::addTask);
        updateDuplicates();
    }

private slots:
    void addTask() {
        auto *row = new SynonymRow(this);
        tasks.append(row);
        tasksLayout->addWidget(row);
        connect(row, &SynonymRow::editRequested, this, &MainWindow::editTask);
        connect(row, &SynonymRow::removeRequested, this, &MainWindow::removeTask);
        qDebug() << "Fila de sinonimos agregada:" << row->synonyms();
        updateDuplicates();
    }

    void editTask(SynonymRow *row) {
        bool accepted = false;
        const QString candidate = QInputDialog::getText(
            this, QStringLiteral("Editar sinónimos"),
            QStringLiteral("Ingrese un par con formato Palabra - Sinonimo:"),
            QLineEdit::Normal, row->synonyms(), &accepted).trimmed();
        if (!accepted) return;

        const QStringList parts = candidate.split('-');
        if (parts.size() != 2 || parts[0].trimmed().isEmpty() || parts[1].trimmed().isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("Formato incorrecto"),
                                 QStringLiteral("Use el formato Palabra - Sinonimo, con ambas palabras."));
            return;
        }

        const QString normalized = parts[0].trimmed() + QStringLiteral(" - ") + parts[1].trimmed();
        for (auto *other : tasks) {
            if (other != row && other->synonyms().compare(normalized, Qt::CaseInsensitive) == 0) {
                QMessageBox::warning(this, QStringLiteral("Sinónimos duplicados"),
                                     QStringLiteral("Ya existe una fila con ese mismo par."));
                return;
            }
        }

        row->setSynonyms(normalized);
        qDebug() << "Fila editada:" << normalized;
        updateDuplicates();
    }

    void removeTask(SynonymRow *row) {
        tasks.removeAll(row);
        tasksLayout->removeWidget(row);
        row->hide();
        row->deleteLater();
        qDebug() << "Fila de sinonimos eliminada";
        updateDuplicates();
    }

private:
    void updateDuplicates() {
        int duplicateRows = 0;
        for (auto *row : tasks) {
            bool duplicate = false;
            for (auto *other : tasks) {
                if (row != other &&
                    row->synonyms().compare(other->synonyms(), Qt::CaseInsensitive) == 0) {
                    duplicate = true;
                    break;
                }
            }
            row->markDuplicate(duplicate);
            if (duplicate) ++duplicateRows;
        }
        statusLabel->setText(QStringLiteral("Checklist de sinónimos: %1 filas (%2 duplicadas)")
                             .arg(tasks.size()).arg(duplicateRows));
    }

    QLabel *statusLabel = nullptr;
    QVBoxLayout *tasksLayout = nullptr;
    QVector<SynonymRow *> tasks;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}

#include "main.moc"
