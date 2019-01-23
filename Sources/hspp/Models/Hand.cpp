// Copyright (c) 2018 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <hspp/Models/Hand.hpp>

namespace Hearthstonepp
{
Hand::Hand()
{
    m_cards.fill(nullptr);
}

Player& Hand::GetOwner() const
{
    return *m_owner;
}

void Hand::SetOwner(Player& owner)
{
    m_owner = &owner;
}
}  // namespace Hearthstonepp