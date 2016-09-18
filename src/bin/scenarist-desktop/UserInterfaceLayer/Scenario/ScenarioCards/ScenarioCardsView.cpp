#include "ScenarioCardsView.h"

#include <3rd_party/Helpers/ShortcutHelper.h>

#include <3rd_party/Widgets/ActivityEdit/gui/activityedit.h>
#include <3rd_party/Widgets/FlatButton/FlatButton.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QVariant>

using UserInterface::ScenarioCardsView;


ScenarioCardsView::ScenarioCardsView(QWidget* _parent) :
	QWidget(_parent),
	m_cardsEdit(new ActivityEdit(_parent)),
	m_addCard(new FlatButton(_parent)),
	m_addNote(new FlatButton(_parent)),
	m_addHLine(new FlatButton(_parent)),
	m_addVLine(new FlatButton(_parent)),
	m_sort(new FlatButton(_parent)),
	m_moveToDraft(new FlatButton(_parent)),
	m_moveToScript(new FlatButton(_parent)),
	m_toolbarSpacer(new QLabel(_parent))
{
	initView();
	initConnections();
	initStyleSheet();
}

void ScenarioCardsView::clear()
{
	m_cardsEdit->clear();
}

void ScenarioCardsView::load(const QString& _xml)
{
	m_cardsEdit->load(_xml);
}

QString ScenarioCardsView::save() const
{
	return m_cardsEdit->save();
}

void ScenarioCardsView::addCard(int _cardType, const QString& _title, const QString& _description)
{
	m_cardsEdit->addCard(_cardType, _title, _description);
}

void ScenarioCardsView::updateCard(int _cardNumber, int _type, const QString& _title, const QString& _description)
{
	m_cardsEdit->updateCard(_cardNumber, _type, _title, _description);
}

void ScenarioCardsView::removeCard(int _cardNumber)
{
	m_cardsEdit->selectCard(_cardNumber);
	m_cardsEdit->deleteSelectedItems();
}

void ScenarioCardsView::selectCard(int _cardNumber)
{
	m_cardsEdit->selectCard(_cardNumber);
}

int ScenarioCardsView::selectedCardNumber() const
{
	return m_cardsEdit->selectedCardNumber();
}

void ScenarioCardsView::setCommentOnly(bool _isCommentOnly)
{
	m_addCard->setEnabled(!_isCommentOnly);
	m_addNote->setEnabled(!_isCommentOnly);
	m_addHLine->setEnabled(!_isCommentOnly);
	m_addVLine->setEnabled(!_isCommentOnly);
	m_sort->setEnabled(!_isCommentOnly);
}

void ScenarioCardsView::initView()
{
	m_addCard->setIcons(QIcon(":/Graphics/Icons/Editing/add.png"));
	m_addCard->setToolTip(tr("Add new card"));

	m_addNote->setIcons(QIcon(":/Graphics/Icons/Review/comment.png"));
	m_addNote->setToolTip(tr("Add note"));

	m_addHLine->setIcons(QIcon(":/Graphics/Icons/Cards/hline.png"));
	m_addHLine->setToolTip(tr("Add horizontal line"));

	m_addVLine->setIcons(QIcon(":/Graphics/Icons/Cards/vline.png"));
	m_addVLine->setToolTip(tr("Add vertical line"));

	m_sort->setIcons(QIcon(":/Graphics/Icons/Cards/table.png"));
	m_sort->setToolTip(tr("Sort cards"));

	m_moveToDraft->setIcons(QIcon(":/Graphics/Icons/Editing/draft.png"));
	m_moveToDraft->setToolTip(tr("Draft"));
	m_moveToDraft->hide();

	m_moveToScript->setIcons(QIcon(":/Graphics/Icons/Mobile/script.png"));
	m_moveToScript->setToolTip(tr("Script"));
	m_moveToScript->hide();

	m_toolbarSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	QWidget* toolbar = new QWidget(this);
	QHBoxLayout* toolbarLayout = new QHBoxLayout(toolbar);
	toolbarLayout->setContentsMargins(QMargins());
	toolbarLayout->setSpacing(0);
	toolbarLayout->addWidget(m_addCard);
	toolbarLayout->addWidget(m_addNote);
	toolbarLayout->addWidget(m_addHLine);
	toolbarLayout->addWidget(m_addVLine);
	toolbarLayout->addWidget(m_sort);
	toolbarLayout->addWidget(m_toolbarSpacer);
	toolbarLayout->addWidget(m_moveToDraft);
	toolbarLayout->addWidget(m_moveToScript);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->setContentsMargins(QMargins());
	layout->setSpacing(0);
	layout->addWidget(toolbar);
	layout->addWidget(m_cardsEdit);

	setLayout(layout);
}

void ScenarioCardsView::initConnections()
{
	connect(m_cardsEdit, &ActivityEdit::schemeChanged, this, &ScenarioCardsView::schemeChanged);

	connect(m_addCard, &FlatButton::clicked, this, &ScenarioCardsView::addCardClicked);
//	connect(m_addCard, &FlatButton::clicked, [=] {
//		m_cardsEdit->addCard(1, "title", "description");
//	});
}

void ScenarioCardsView::initStyleSheet()
{
	m_addCard->setProperty("inTopPanel", true);
	m_addNote->setProperty("inTopPanel", true);
	m_addHLine->setProperty("inTopPanel", true);
	m_addVLine->setProperty("inTopPanel", true);
	m_sort->setProperty("inTopPanel", true);
	m_moveToDraft->setProperty("inTopPanel", true);
	m_moveToScript->setProperty("inTopPanel", true);

	m_toolbarSpacer->setProperty("inTopPanel", true);
	m_toolbarSpacer->setProperty("topPanelTopBordered", true);

	m_cardsEdit->setProperty("mainContainer", true);
}
