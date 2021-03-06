#ifndef SCENARIOTEXTEDITMANAGER_H
#define SCENARIOTEXTEDITMANAGER_H

#include <QObject>

class QMenu;
class QTextCursor;


namespace UserInterface {
    class ScenarioTextEditWidget;
}

namespace BusinessLogic {
    class ScenarioTextDocument;
}

namespace ManagementLayer
{
    /**
     * @brief Управляющий редактированием сценария
     */
    class ScenarioTextEditManager : public QObject
    {
        Q_OBJECT

    public:
        explicit ScenarioTextEditManager(QObject* _parent, QWidget* _parentWidget);

        QWidget* toolbar() const;
        QWidget* view() const;

        /**
         * @brief Установить документ для редактирования
         */
        void setScenarioDocument(BusinessLogic::ScenarioTextDocument* _document, bool _isDraft = false);

        /**
         * @brief Установить хронометраж
         */
        void setDuration(const QString& _duration);

        /**
         * @brief Установить значения счётчиков
         */
        void setCountersInfo(const QString& _counters);

        /**
         * @brief Установить позицию курсора
         */
        void setCursorPosition(int _position);

        /**
         * @brief Перезагрузить параметры текстового редактора
         */
        void reloadTextEditSettings();

        /**
         * @brief Получить текущую позицию курсора
         */
        int cursorPosition() const;

        /**
         * @brief Установить список дополнительных курсоров для отрисовки
         */
        void setAdditionalCursors(const QMap<QString, int>& _cursors);

        /**
         * @brief Установить режим работы со сценарием
         */
        void setCommentOnly(bool _isCommentOnly);

        /**
         * @brief Пролистать сценарий, чтобы был виден заданный курсор соавтора
         */
        void scrollToAdditionalCursor(int _additionalCursorIndex);

        /**
         * @brief Пролистать сценарий, чтобы был виден заданный курсор
         */
        void scrollToPosition(int _position);

#ifdef Q_OS_MAC
        /**
         * @brief Сформировать меню "Правка"
         */
        void buildEditMenu(QMenu* _menu);
#endif

    public slots:
        /**
         * @brief Добавить элемент сценария в указанной позиции
         */
        /** @{ */
        void addScenarioItemFromCards(int _position, int _type, const QString& _title,
            const QColor& _color, const QString& _description);
        void addScenarioItem(int _position, int _type, const QString& _header,
            const QColor& _color, const QString& _description);
        /** @{ */

        /**
         * @brief Изменить элемент сценария
         */
        void editScenarioItem(int _startPosition, int _endPosition, int _type,
            const QString& _title, const QString& _colors, const QString& _description);

        /**
         * @brief Удалить заданный текст сценария
         */
        void removeScenarioText(int _from, int _to);

        /**
         * @brief Установить в заданной позиции заданный тип блока
         */
        void changeItemType(int _position, int _type);

        /**
         * @brief Включить/выключить дзен режим
         */
        void setZenMode(bool _isZen);

    signals:
        /**
         * @brief Изменился текст сценария
         */
        void textChanged();

        /**
         * @brief Изменилась позиция курсора
         */
        void cursorPositionChanged(int _position);

        /**
         * @brief Запрос отмены действия
         */
        void undoRequest();

        /**
         * @brief Запрос повтора действия
         */
        void redoRequest();

        /**
         * @brief Изменился режим отображения сценария
         */
        void textModeChanged();

        /**
         * @brief Выйти из дзен режима
         */
        void quitFromZenMode();

    private slots:
        /**
         * @brief Реакция на изменение коэффициента масштабирования редактора сценария
         */
        void aboutTextEditZoomRangeChanged(qreal _zoomRange);

    private:
        /**
         * @brief Настроить представление
         */
        void initView();

        /**
         * @brief Настроить соединения
         */
        void initConnections();

    private:
        /**
         * @brief Редактор
         */
        UserInterface::ScenarioTextEditWidget* m_view;
    };
}

#endif // SCENARIOTEXTEDITMANAGER_H
