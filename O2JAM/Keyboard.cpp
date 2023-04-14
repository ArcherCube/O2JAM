#include "Keyboard.h"

#include "CommonType.h"
#include "CentralTimer.h"
#include "Key.h"

#include "Message.h"


Keyboard::Keyboard()
	:Actor(KEYBOARD_PATH, KEYBOARD_POSITION, KEYBOARD_Z)
	, keyHashmap(new KeyHashmap())
{
	keyHashmap->insert({ KeyName::A,new Key(Key::KeyType::WHITE_L, KEY_A_POSITION) });
	keyHashmap->insert({ KeyName::S,new Key(Key::KeyType::BLUE,  KEY_S_POSITION) });
	keyHashmap->insert({ KeyName::D,new Key(Key::KeyType::WHITE_R,  KEY_D_POSITION) });
	keyHashmap->insert({ KeyName::SPACE,new Key(Key::KeyType::YELLOW,  KEY_SPACE_POSITION) });
	keyHashmap->insert({ KeyName::J,new Key(Key::KeyType::WHITE_L,  KEY_J_POSITION) });
	keyHashmap->insert({ KeyName::K,new Key(Key::KeyType::BLUE,  KEY_K_POSITION) });
	keyHashmap->insert({ KeyName::L,new Key(Key::KeyType::WHITE_R,  KEY_L_POSITION) });
}


const TimestampType Keyboard::Press(const KeyName& key) const
{
	KeyHashmapCiter cIter = keyHashmap->find(key);

	if (cIter == keyHashmap->cend()) return 0;

	return cIter->second->Press();
}

const TimestampType Keyboard::Release(const KeyName& key) const
{
	KeyHashmapCiter cIter = keyHashmap->find(key);

	if (cIter == keyHashmap->cend()) return 0;

	return cIter->second->Release();
}

const TimestampType Keyboard::GetLastHoldTime(const KeyName& key) const
{
	KeyHashmapCiter cIter = keyHashmap->find(key);

	if (cIter == keyHashmap->cend()) return 0;

	return cIter->second->GetLastHoldTime();
}


Keyboard::~Keyboard()
{
	for (const auto& keyPair : *keyHashmap)
	{
		delete keyPair.second;
	}
	keyHashmap->clear();
	delete keyHashmap;
}
