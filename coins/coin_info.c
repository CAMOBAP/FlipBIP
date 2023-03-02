// This file is automatically generated from coin_info.c.mako

// DO NOT EDIT



#include "coins.h"



#include "../crypto/curves.h"

#include "../crypto/secp256k1.h"



const CoinInfo coins[COINS_COUNT] = {

{

	.coin_name = "Bitcoin",

	.coin_shortcut = "BTC",

	.maxfee_kb = 2000000,

	.signed_message_header = "\x18" "Bitcoin Signed Message:\n",

	.has_segwit = true,

	.has_taproot = true,

	.has_fork_id = false,

	.force_bip143 = false,

	.decred = false,

	.decimals = 8,

	.address_type = 0,

	.address_type_p2sh = 5,

	.xpub_magic = 0x0488b21e,

	.xpub_magic_segwit_p2sh = 0x049d7cb2,

	.xpub_magic_segwit_native = 0x04b24746,

	.xpub_magic_multisig_segwit_p2sh = 0x0295b43f,

	.xpub_magic_multisig_segwit_native = 0x02aa7ed3,

	.fork_id = 0,

	.bech32_prefix = "bc",

	.cashaddr_prefix = NULL,

	.coin_type = (0 | 0x80000000),

	.negative_fee = false,

	.curve_name = SECP256K1_NAME,

	.curve = &secp256k1_info,

	.extra_data = false,

	.timestamp = false,

	.overwintered = false,

},

{

	.coin_name = "Regtest",

	.coin_shortcut = "REGTEST",

	.maxfee_kb = 10000000,

	.signed_message_header = "\x18" "Bitcoin Signed Message:\n",

	.has_segwit = true,

	.has_taproot = true,

	.has_fork_id = false,

	.force_bip143 = false,

	.decred = false,

	.decimals = 8,

	.address_type = 111,

	.address_type_p2sh = 196,

	.xpub_magic = 0x043587cf,

	.xpub_magic_segwit_p2sh = 0x044a5262,

	.xpub_magic_segwit_native = 0x045f1cf6,

	.xpub_magic_multisig_segwit_p2sh = 0x024289ef,

	.xpub_magic_multisig_segwit_native = 0x02575483,

	.fork_id = 0,

	.bech32_prefix = "bcrt",

	.cashaddr_prefix = NULL,

	.coin_type = (1 | 0x80000000),

	.negative_fee = false,

	.curve_name = SECP256K1_NAME,

	.curve = &secp256k1_info,

	.extra_data = false,

	.timestamp = false,

	.overwintered = false,

},

{

	.coin_name = "Testnet",

	.coin_shortcut = "TEST",

	.maxfee_kb = 10000000,

	.signed_message_header = "\x18" "Bitcoin Signed Message:\n",

	.has_segwit = true,

	.has_taproot = true,

	.has_fork_id = false,

	.force_bip143 = false,

	.decred = false,

	.decimals = 8,

	.address_type = 111,

	.address_type_p2sh = 196,

	.xpub_magic = 0x043587cf,

	.xpub_magic_segwit_p2sh = 0x044a5262,

	.xpub_magic_segwit_native = 0x045f1cf6,

	.xpub_magic_multisig_segwit_p2sh = 0x024289ef,

	.xpub_magic_multisig_segwit_native = 0x02575483,

	.fork_id = 0,

	.bech32_prefix = "tb",

	.cashaddr_prefix = NULL,

	.coin_type = (1 | 0x80000000),

	.negative_fee = false,

	.curve_name = SECP256K1_NAME,

	.curve = &secp256k1_info,

	.extra_data = false,

	.timestamp = false,

	.overwintered = false,

},

};
