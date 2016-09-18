#include "ScenarioCardsManager.h"

#include <Domain/Scenario.h>

#include <BusinessLayer/ScenarioDocument/ScenarioModel.h>

#include <DataLayer/DataStorageLayer/StorageFacade.h>
#include <DataLayer/DataStorageLayer/ScenarioStorage.h>

#include <UserInterfaceLayer/Scenario/ScenarioCards/ScenarioCardsView.h>
#include <UserInterfaceLayer/Scenario/ScenarioItemDialog/ScenarioSchemeItemDialog.h>

using ManagementLayer::ScenarioCardsManager;
using UserInterface::ScenarioCardsView;
using UserInterface::ScenarioSchemeItemDialog;

ScenarioCardsManager::ScenarioCardsManager(QObject* _parent, QWidget* _parentWidget) :
	QObject(_parent),
	m_view(new ScenarioCardsView(_parentWidget)),
	m_addItemDialog(new ScenarioSchemeItemDialog(_parentWidget)),
	m_scenario(nullptr),
	m_model(nullptr)
{
	initConnections();
}

QWidget* ScenarioCardsManager::view() const
{
	return m_view;
}

QString ScenarioCardsManager::save() const
{
	return m_view->save();
}
#include <QDebug>
void ScenarioCardsManager::load(BusinessLogic::ScenarioModel* _model, const QString& _xml)
{
	//
	// Сохраним модель
	//
	if (m_model != _model) {
		m_model = _model;
		connect(m_model, &BusinessLogic::ScenarioModel::rowsInserted, [=] (const QModelIndex& _parent, int _first, int _last) {
			qDebug() << "insert rows from" << _first << "to" << _last << "in parent" << _parent;
			for (int row = _first; row <= _last; ++row) {
				const QModelIndex index = m_model->index(row, 0, _parent);
				BusinessLogic::ScenarioModelItem* item = m_model->itemForIndex(index);
				QModelIndex currentCardIndex = _parent;
				if (_first > 0) {
					//
					// -1 т.к. нужен предыдущий элемент
					//
					const int itemRow = _first - 1;
					if (_parent.isValid()) {
						currentCardIndex = _parent.child(itemRow, 0);
					} else {
						currentCardIndex = m_model->index(itemRow, 0);
					}
				}
				m_view->selectCard(m_model->numberForIndex(currentCardIndex));
				m_view->addCard(
					item->type(),
					item->title().isEmpty() ? item->header() : item->title(),
					item->description());
			}
		});
		connect(m_model, &BusinessLogic::ScenarioModel::rowsRemoved, [=] (const QModelIndex& _parent, int _first, int _last) {
			qDebug() << "remove rows from" << _first << "to" << _last << "in parent" << _parent;
			for (int row = _last; row >= _first; --row) {
				QModelIndex currentCardIndex = _parent;
				if (_parent.isValid()) {
					currentCardIndex = _parent.child(row, 0);
				} else {
					currentCardIndex = m_model->index(row, 0);
				}
				m_view->removeCard(m_model->numberForIndex(currentCardIndex));
			}
		});
		connect(m_model, &BusinessLogic::ScenarioModel::dataChanged, [=] (const QModelIndex& _topLeft, const QModelIndex& _bottomRight) {
			for (int row = _topLeft.row(); row <= _bottomRight.row(); ++row) {
				const QModelIndex index = m_model->index(row, 0, _topLeft.parent());
				const BusinessLogic::ScenarioModelItem* item = m_model->itemForIndex(index);
				m_view->updateCard(
					m_model->numberForIndex(index),
					item->type(),
					item->title().isEmpty() ? item->header() : item->title(),
					item->description());
			}
		});
	}

	qDebug() << "************************************";

	//
	// Загрузим сценарий
	//
	// ... если схема есть, то просто загружаем её
	//
	if (!_xml.isEmpty()) {
		m_view->load(_xml);
	}
	//
	// ... а если схема пуста, сформируем её на основе модели
	//
	else {
		m_view->load(m_model->simpleScheme());
	}
}

void ScenarioCardsManager::clear()
{
	m_model->disconnect();
	m_model = nullptr;
	m_view->clear();
}

void ScenarioCardsManager::setCommentOnly(bool _isCommentOnly)
{
	m_view->setCommentOnly(_isCommentOnly);
}

void ScenarioCardsManager::addCard()
{
	m_addItemDialog->clear();

	//
	// Если пользователь действительно хочет добавить элемент
	//
	if (m_addItemDialog->exec() == QLightBoxDialog::Accepted) {
		const int type = m_addItemDialog->cardType();
		const QString title = m_addItemDialog->cardTitle();
		const QString description = m_addItemDialog->cardDescription();

		//
		// Если задан заголовок
		//
		if (!title.isEmpty()) {
			//
			// Определим карточку, после которой нужно добавить элемент
			//
			QModelIndex index;
			const int selectedItemNumber = m_view->selectedCardNumber();
			if (selectedItemNumber != -1) {
				index = m_model->indexForNumber(selectedItemNumber);
			}

			emit addItemRequest(index, type, title, description);
		}
	}
}

void ScenarioCardsManager::initConnections()
{
	connect(m_view, &ScenarioCardsView::addCardClicked, this, &ScenarioCardsManager::addCard);

	connect(m_view, &ScenarioCardsView::schemeChanged, this, &ScenarioCardsManager::schemeChanged);
}
