#ifndef LANGUAGEDIALOG_H
#define LANGUAGEDIALOG_H

#include <3rd_party/Widgets/QLightBoxWidget/qlightboxdialog.h>

namespace Ui {
	class LanguageDialog;
}

namespace UserInterface
{
	/**
	 * @brief Диалог выбора языка программы
	 */
	class LanguageDialog : public QLightBoxDialog
	{
		Q_OBJECT

	public:
		explicit LanguageDialog(QWidget *parent = 0, int _language = -1);
		~LanguageDialog();

		/**
		 * @brief Выбранный язык
		 */
		int language() const;

	private:
		/**
		 * @brief Настроить представление
		 */
        void initView() override;

		/**
		 * @brief Настроить соединения
		 */
        void initConnections() override;

	private:
		/**
		 * @brief Интерфейс
		 */
		Ui::LanguageDialog *m_ui;
	};
}

#endif // LANGUAGEDIALOG_H
