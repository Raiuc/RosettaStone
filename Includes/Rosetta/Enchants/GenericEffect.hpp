// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#ifndef ROSETTASTONE_GENERIC_EFFECT_HPP
#define ROSETTASTONE_GENERIC_EFFECT_HPP

#include <Rosetta/Enchants/Attrs/Attr.hpp>
#include <Rosetta/Enchants/IEffect.hpp>
#include <Rosetta/Models/Player.hpp>
#include <Rosetta/Games/Game.hpp>

namespace RosettaStone
{
//!
//! \brief GeneralEffect class.
//!
template <typename T = Entity, typename AttrT = Attr<T>>
class GenericEffect : public IEffect
{
 public:
    GenericEffect(AttrT* attr, EffectOperator effectOp, int value)
        : m_attr(attr), m_effectOp(effectOp), m_value(value)
    {
        // Do nothing
    }

    //! Applies effect to the target entity.
    //! \param entity An entity to which effect is applied.
    //! \param isOneTurnEffect Whether effect lasts only one turn.
    void ApplyTo(Entity* entity, bool isOneTurnEffect = false) const override
    {
        if (isOneTurnEffect)
        {
            entity->owner->GetGame()->oneTurnEffects.emplace_back(entity, this);
        }

        m_attr->Apply(entity, m_effectOp, m_value);
    }

    //! Applies aura effect to the target entity.
    //! \param entity An entity to which aura effect is applied.
    void ApplyAuraTo(Entity* entity) const override
    {
        m_attr->ApplyAura(entity, m_effectOp, m_value);
    }

    //! Removes effect from the target entity.
    //! \param entity An entity to which effect is removed.
    void RemoveFrom(Entity* entity) const override
    {
        m_attr->Remove(entity, m_effectOp, m_value);
    }

    //! Removes aura effect from the target entity.
    //! \param entity An entity to which aura effect is removed.
    void RemoveAuraFrom(Entity* entity) const override
    {
        m_attr->RemoveAura(entity, m_effectOp, m_value);
    }

    //! Creates a new Effect having changed amount of \p newValue.
    //! \param newValue A value to change.
    //! \return A new Effect having changed amount.
    IEffect* ChangeValue(int newValue) const override
    {
        return new GenericEffect<T, AttrT>(m_attr, m_effectOp, newValue);
    }

 private:
    AttrT* m_attr = nullptr;
    EffectOperator m_effectOp;
    int m_value;
};
}  // namespace RosettaStone

#endif  // ROSETTASTONE_GENERAL_EFFECT_HPP
