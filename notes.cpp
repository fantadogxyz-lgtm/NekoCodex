#include "notes.h"
#include "qgridlayout.h"
#include "ui_notes.h"

Notes::Notes(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Notes),
    m_gridLayout(nullptr),
    m_firstShow(true)
{
    ui->setupUi(this);

    setupGrid();

    connect(ui->createNoteBut, &QPushButton::clicked,
        this, &Notes::onCreateNoteClicked);

    loadNotes();

    qDebug() << "=== created NOTES :3 ===";

}

Notes::~Notes()
{
    delete ui;
}

void Notes::setupGrid() {
    QWidget* container = ui->scrollArea->widget();

    if (container->layout()) {
        delete container->layout();
    }

    m_gridLayout = new QGridLayout(container);

    int scrollWidth = ui->scrollArea->width(); 
    int totalWidth = scrollWidth - 20; // минус скроллбар

    int spacing = (totalWidth - (COLUMNS * NOTE_WIDTH)) / (COLUMNS + 1);

    m_gridLayout->setHorizontalSpacing(spacing);
    m_gridLayout->setVerticalSpacing(20);
    m_gridLayout->setContentsMargins(spacing, 20, spacing, 20);
    

    for (int i = 0; i < COLUMNS; ++i) {
        m_gridLayout->setColumnStretch(i, 1);
    }

    // Растяжка для последнего ряда
    //m_gridLayout->setRowStretch(0, 0);

   
}

void Notes::loadNotes() {
    cleanNotes();

    auto* dbManager = DatabaseNoteManager::instance();
    if (!dbManager->getDB().isOpen()) {
        qDebug() << "loadNotes(): Database not open, initializing...";
        if (!dbManager->initialize()) {
            qCritical() << "loadNotes(): Failed to initialize database!";
            return;
        }
    }

    auto notes = DatabaseNoteManager::instance()->getAllNotes();

    // Сортировочка от свежих к старым
    std::sort(notes.begin(), notes.end(),
        [](const NoteData& a, const NoteData& b) {
            return a.createdAt > b.createdAt;
        });

    for (const auto& noteData : notes) {
        auto* note = new OneNote();
        note->setData(noteData.id, noteData.title, noteData.content, noteData.createdAt, noteData.updatedAt, noteData.color);

        note->setFixedSize(NOTE_WIDTH, NOTE_HEIGHT);
        connect(note, &OneNote::noteChanged,
            this, &Notes::onNoteChanged);
        connect(note, &OneNote::noteDeleted,
            this, &Notes::onNoteDeleted);

        connect(note, &OneNote::notesColorChanged, 
            this, &Notes::onNoteColorChanged);

        m_notes.prepend(note);
    }

    arrangeNotes(); 
}

void Notes::arrangeNotes()
{
    if (!m_gridLayout) {
        return;
    }

    while (auto* item = m_gridLayout->takeAt(0)) {
        m_gridLayout->removeItem(item);
        delete item; 
    }

    for (int i = 0; i < m_notes.size(); ++i) {
        int row = i / COLUMNS;
        int col = i % COLUMNS;

        m_gridLayout->addWidget(m_notes[i], row, col, Qt::AlignCenter);
    }


    if (!m_notes.isEmpty()) {
        int lastRow = (m_notes.size() - 1) / COLUMNS;
        m_gridLayout->setRowStretch(lastRow + 1, 1);
    }

    ui->scrollArea->widget()->update();
}

void Notes::cleanNotes()
{
    for (auto* note : m_notes) {
        note->deleteLater();
    }
    m_notes.clear();
}

void Notes::onCreateNoteClicked()
{
    int newId = DatabaseNoteManager::instance()->createNote("", "");

    if (newId > 0) {
        auto noteData = DatabaseNoteManager::instance()->getNoteById(newId);

        auto* note = new OneNote();
        note->setData(noteData.id, noteData.title, noteData.content,
            noteData.createdAt, noteData.updatedAt, noteData.color);
        note->setFixedSize(NOTE_WIDTH, NOTE_HEIGHT);

        connect(note, &OneNote::noteChanged,
            this, &Notes::onNoteChanged);
        connect(note, &OneNote::noteDeleted,
            this, &Notes::onNoteDeleted);
        connect(note, &OneNote::notesColorChanged,
            this, &Notes::onNoteColorChanged);

        m_notes.append(note);
        arrangeNotes();


        QTimer::singleShot(100, this, [this, note]() {
            ui->scrollArea->ensureWidgetVisible(note);
            qDebug() << "Scrolling to new note";
            });
    }
}

void Notes::onNoteChanged(int id, const QString& title, const QString& content)
{
    if (DatabaseNoteManager::instance()->updateNote(id, title, content)) {
       // loadNotes();
    }
}

void Notes::onNoteDeleted(int id)
{
    int index = -1;
    for (int i = 0; i < m_notes.size(); ++i) {
        if (m_notes[i]->getId() == id) {
            index = i;
            break;
        }
    }

    if (index != -1 && DatabaseNoteManager::instance()->deleteNote(id)) {
        m_notes[index]->deleteLater();
        m_notes.remove(index);
        arrangeNotes();
    }
}

void Notes::onNoteColorChanged(int id, const QString& color) {
    if (DatabaseNoteManager::instance()->updateNoteColor(id, color)) {
        qDebug() << "Color was changed! - from notes";
        // loadNotes();
    }
}


void Notes::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    if (m_firstShow) {
        m_firstShow = false;

        // Перестраиваем все с реальными размерами
        setupGrid();
        arrangeNotes();
    }
}
