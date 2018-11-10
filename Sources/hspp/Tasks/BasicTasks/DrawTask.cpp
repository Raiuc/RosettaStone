// Copyright (c) 2018 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <hspp/Cards/Entity.h>
#include <hspp/Tasks/BasicTasks/DrawTask.h>

namespace Hearthstonepp::BasicTasks
{
DrawTask::DrawTask(TaskAgent& agent, size_t num) : m_agent(agent), m_num(num)
{
    // Do nothing
}

TaskID DrawTask::GetTaskID() const
{
    return TaskID::DRAW;
}

MetaData DrawTask::Impl(Player& user, Player&)
{
    size_t num = m_num;
    MetaData result = MetaData::DRAW_SUCCESS;

    std::vector<Entity*>& deck = user.cards;
    std::vector<Entity*>& hand = user.hand;

    // After reaching fatigue
    if (deck.size() < num)
    {
        const size_t numDrawAfterFatigue = num - deck.size();

        // Sigma (i = 1 to numDrawAfterFatigue) { current.exhausted + i }
        const auto fatigueDamage = static_cast<int>(
            user.exhausted * numDrawAfterFatigue +
            numDrawAfterFatigue * (numDrawAfterFatigue + 1) / 2);
        const int remainHealth =
            static_cast<int>(user.hero->health) - fatigueDamage;

        user.hero->health =
            remainHealth > 0 ? static_cast<size_t>(remainHealth) : 0;
        user.exhausted += static_cast<BYTE>(numDrawAfterFatigue);

        num = deck.size();
        result = MetaData::DRAW_EXHAUST;
    }

    // When hand size over 10, over draw
    if (hand.size() + num > 10)
    {
        // The number of over draw
        const size_t over = hand.size() + num - 10;

        std::vector<Entity*> burnt;
        burnt.reserve(over);

        // draw burnt card
        for (size_t i = 0; i < over; ++i)
        {
            burnt.emplace_back(deck.back());
            deck.pop_back();
        }

        num = 10 - hand.size();

        if (result == MetaData::DRAW_EXHAUST)
        {
            result = MetaData::DRAW_EXHAUST_OVERDRAW;
        }
        else
        {
            result = MetaData::DRAW_OVERDRAW;
        }

        // Send burnt cards to GameInterface
        const TaskMetaTrait trait(TaskID::OVER_DRAW, result, user.id);
        m_agent.Notify(Serializer::CreateEntityVector(trait, burnt));
    }

    // Draw success
    for (size_t i = 0; i < num; ++i)
    {
        hand.push_back(deck.back());
        deck.pop_back();
    }

    return result;
}

DrawCardTask::DrawCardTask(Card& card) : m_card(card)
{
    // Do nothing
}

TaskID DrawCardTask::GetTaskID() const
{
    return TaskID::DRAW;
}

MetaData DrawCardTask::Impl(Player& user, Player&)
{
    std::vector<Entity*>& deck = user.cards;
    std::vector<Entity*>& hand = user.hand;

    switch (m_card.cardType)
    {
        case +CardType::MINION:
            hand.emplace_back(new Minion(m_card));
            break;
        case +CardType::WEAPON:
            hand.emplace_back(new Weapon(m_card));
            break;
        default:
            hand.emplace_back(new Entity(m_card));
    }

    if (!deck.empty())
    {
        deck.pop_back();
    }

    return MetaData::DRAW_SUCCESS;
}
}  // namespace Hearthstonepp::BasicTasks