// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

bool CheckProofOfWorkGen(uint256 hash, unsigned int nBits)
{
    bool fNegative;
    bool fOverflow;
    uint256 bnTarget;

    //if (Params().SkipProofOfWorkCheck())
    //    return true;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > (~uint256(0) >> 20))
        return false;

    // Check proof of work matches claimed amount
    if (hash > bnTarget)
        return false;

    return true;
}

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x000000f351b8525f459c879f1e249b5d3d421b378ac6b760ea8b8e0df2454f33"));
    //(424998, uint256("f31e381eedb0ed3ed65fcc98cc71f36012bee32e8efd017c4f9fb0620fd35f6b"))
    //(616764, uint256("29dd0bd1c59484f290896687b4ffb6a49afa5c498caf61967c69a541f8191557")) //first block to use modifierV2
    //(623933, uint256("c7aafa648a0f1450157dc93bd4d7448913a85b7448f803b4ab970d91fc2a7da7"))
    //(791150, uint256("8e76f462e4e82d1bd21cb72e1ce1567d4ddda2390f26074ffd1f5d9c270e5e50"))
    //(795000, uint256("4423cceeb9fd574137a18733416275a70fdf95283cc79ad976ca399aa424a443"))
    //(863787, uint256("5b2482eca24caf2a46bb22e0545db7b7037282733faa3a42ec20542509999a64"))
    //(863795, uint256("2ad866818c4866e0d555181daccc628056216c0db431f88a825e84ed4f469067"))
    //(863805, uint256("a755bd9a22b63c70d3db474f4b2b61a1f86c835b290a081bb3ec1ba2103eb4cb"))
    //(867733, uint256("03b26296bf693de5782c76843d2fb649cb66d4b05550c6a79c047ff7e1c3ae15"))
    //(879650, uint256("227e1d2b738b6cd83c46d1d64617934ec899d77cee34336a56e61b71acd10bb2"))
    //(895400, uint256("7796a0274a608fac12d400198174e50beda992c1d522e52e5b95b884bc1beac6"))//block that serial# range is enforced
    //(895991, uint256("d53013ed7ea5c325b9696c95e07667d6858f8ff7ee13fecfa90827bf3c9ae316"))//network split here
    //(908000, uint256("202708f8c289b676fceb832a079ff6b308a28608339acbf7584de533619d014d"));
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1520007171, // * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x000000f351b8525f459c879f1e249b5d3d421b378ac6b760ea8b8e0df2454f33"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1520007171,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x79ba0d9d15d36edee8d07cc300379ec65ab7e12765acd883e870aa618dbcc1a8"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1518723178,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params() const
{
    assert(this);
    static CBigNum bnTrustedModulus(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParams = libzerocoin::ZerocoinParams(bnTrustedModulus);

    return &ZCParams;
}

class CMainParams : public CChainParams
{
public:
	CMainParams()
	{
		networkID = CBaseChainParams::MAIN;
		strNetworkID = "main";
		/**
		* The message start string is designed to be unlikely to occur in normal data.
		* The characters are rarely used upper ASCII, not valid as UTF-8, and produce
		* a large 4-byte int at any alignment.
		*/

		pchMessageStart[0] = 0xdf;
		pchMessageStart[1] = 0xa0;
		pchMessageStart[2] = 0x8d;
		pchMessageStart[3] = 0x8a;
		premineAmt = 620000000;

		vAlertPubKey = ParseHex("046e70d194b1b6b63b9c5431ea83c7b17d0db8930408b1e7937e41759a799e8fcd22d99ffc0c880094bb07a852a9020f810068417e65d19def8ffbdfa90727b637");
		nDefaultPort = 51472;
		bnProofOfWorkLimit = ~uint256(0) >> 20; // DIVI starting difficulty is 1 / 2^12
		nSubsidyHalvingInterval = 210000;
		nMaxReorganizationDepth = 100;
		nEnforceBlockUpgradeMajority = 750;
		nRejectBlockOutdatedMajority = 950;
		nToCheckBlockUpgradeMajority = 1000;
		nMinerThreads = 0;
		nTargetTimespan = 1 * 60; // DIVI: 1 day
		nTargetSpacing = 1 * 60;  // DIVI: 1 minute
		nMaturity = 1; //byrdset from 100
		nMasternodeCountDrift = 20;
		nMaxMoneyOut = 2534320700 * COIN;

		/** Height or Time Based Activations **/
		nLastPOWBlock = 150;
		nModifierUpdateBlock = 615800;
		nZerocoinStartHeight = 863787;
		nZerocoinStartTime = 1533945600; // October 17, 2017 4:30:00 AM
		nBlockEnforceSerialRange = 895400; //Enforce serial range starting this block
		nBlockRecalculateAccumulators = 908000; //Trigger a recalculation of accumulators
		nBlockFirstFraudulent = 891737; //First block that bad serials emerged
		nBlockLastGoodCheckpoint = 891730; //Last valid accumulator checkpoint
		nBlockEnforceInvalidUTXO = 902850; //Start enforcing the invalid UTXO's

		/**
		* Build the genesis block. Note that the output of the genesis coinbase cannot
		* be spent as it did not originally exist in the database.
		*
		* CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
		*   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
		*     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
		*     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
		*   vMerkleTree: e0028e
		*/
		const char* pszTimestamp = "March 2, 2018 - East And West, Both Coasts Brace For Major Winter Storms";
		CMutableTransaction txNew;
		txNew.vin.resize(1);
		txNew.vout.resize(1);
		txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
		txNew.vout[0].nValue = 8000250 * COIN;
		txNew.vout[0].scriptPubKey = CScript() << ParseHex("04bcc3ef3417ba00ab55e3de807a776ade43cbd634a7e2cff383fecc6920cf918b2ad427f6b0a3f8d38f5a41d5dcbf35b394521bd08fcb5f40749df5bfe7d42fe2") << OP_CHECKSIG;
		genesis.vtx.push_back(txNew);
		genesis.hashPrevBlock = 0;
		genesis.hashMerkleRoot = genesis.BuildMerkleTree();
		genesis.nVersion = 1;
		genesis.nTime = 1520007171;
		genesis.nBits = 0x1e0ffff0;
		genesis.nNonce = 416509442;
		genesis.payee = txNew.vout[0].scriptPubKey;

		/*
		hashGenesisBlock = uint256("0x01");
		if (true && genesis.GetHash() != hashGenesisBlock)
		{
		//genesis.nNonce = 1000000;
		printf("recalculating params for mainnet.\n");
		printf("old mainnet genesis nonce: %d\n", genesis.nNonce);
		printf("old mainnet genesis hash:  %s\n", hashGenesisBlock.ToString().c_str());
		// deliberately empty for loop finds nonce value.
		for(genesis.nNonce == 0; !CheckProofOfWorkGen(genesis.GetHash(),genesis.nBits); genesis.nNonce++){ }
		printf("new mainnet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
		printf("new mainnet genesis nonce: %d\n", genesis.nNonce);
		printf("new mainnet genesis hash: %s\n", genesis.GetHash().ToString().c_str());
		}
		*/

		hashGenesisBlock = genesis.GetHash();
		assert(hashGenesisBlock == uint256("0x000008ae5cfd29b6680b53c7061840fdeea45259bd5320322a541a0e4305728a"));
		assert(genesis.hashMerkleRoot == uint256("0xb68f3b6cefa827045e8bac505203050c9d247c10d7fe2a951575924427a51052"));

		//vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "divi.seed.fuzzbawls.pw"));     // Primary DNS Seeder from Fuzzbawls
		//vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "divi.seed2.fuzzbawls.pw"));    // Secondary DNS Seeder from Fuzzbawls
		//vSeeds.push_back(CDNSSeedData("coin-server.com", "coin-server.com"));         // Single node address
		//vSeeds.push_back(CDNSSeedData("s3v3nh4cks.ddns.net", "s3v3nh4cks.ddns.net")); // Single node address
		//vSeeds.push_back(CDNSSeedData("178.254.23.111", "178.254.23.111"));           // Single node address
		vFixedSeeds.clear();
		vSeeds.clear();

		base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 30);
		base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 13);
		base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 212);
		base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
		base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
		// 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
		base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

		convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

		fRequireRPCPassword = true;
		fMiningRequiresPeers = false;
		fAllowMinDifficultyBlocks = false;
		fDefaultConsistencyChecks = false;
		fRequireStandard = true;
		fMineBlocksOnDemand = false;
		fSkipProofOfWorkCheck = false;
		fTestnetToBeDeprecatedFieldRPC = false;
		fHeadersFirstSyncingActive = false;

		nPoolMaxTransactions = 3;
		strSporkKey = "04B433E6598390C992F4F022F20D3B4CBBE691652EE7C48243B81701CBDB7CC7D7BF0EE09E154E6FCBF2043D65AF4E9E97B89B5DBAF830D83B9B7F469A6C45A717";
		strObfuscationPoolDummyAddress = "D87q2gC9j6nNrnzCsg4aY6bHMLsT9nUhEw";
		nStartMasternodePayments = 1533945600; //Wed, 11 Aug 2018 00:00:00 GMT

											   /** Zerocoin */
		zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
			"4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
			"6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
			"7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
			"8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
			"31438167899885040445364023527381951378636564391212010397122822120720357";
		nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
		nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
		nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
		nRequiredAccumulation = 2;
		nDefaultSecurityLevel = 100; //full security level for accumulators
		nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
		nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
	}

	const Checkpoints::CCheckpointData& Checkpoints() const
	{
		return data;
	}
};
static CMainParams mainParams;

class CBetaParams : public CChainParams
{
public:
	CBetaParams()
	{
		networkID = CBaseChainParams::MAIN;
		strNetworkID = "main";
		/**
		* The message start string is designed to be unlikely to occur in normal data.
		* The characters are rarely used upper ASCII, not valid as UTF-8, and produce
		* a large 4-byte int at any alignment.
		*/

		pchMessageStart[0] = 0xdf;
		pchMessageStart[1] = 0xa0;
		pchMessageStart[2] = 0x8d;
		pchMessageStart[3] = 0x8a;
		premineAmt = 2534320700;

		vAlertPubKey = ParseHex("046e70d194b1b6b63b9c5431ea83c7b17d0db8930408b1e7937e41759a799e8fcd22d99ffc0c880094bb07a852a9020f810068417e65d19def8ffbdfa90727b637");
		nDefaultPort = 51472;
		bnProofOfWorkLimit = ~uint256(0) >> 20; // DIVI starting difficulty is 1 / 2^12
		nSubsidyHalvingInterval = 210000;
		nMaxReorganizationDepth = 100;
		nEnforceBlockUpgradeMajority = 750;
		nRejectBlockOutdatedMajority = 950;
		nToCheckBlockUpgradeMajority = 1000;
		nMinerThreads = 0;
		nTargetTimespan = 1 * 60; // DIVI: 1 day
		nTargetSpacing = 1 * 60;  // DIVI: 1 minute
		nMaturity = 1; //byrdset from 100
		nMasternodeCountDrift = 20;
		nMaxMoneyOut = 2535000000 * COIN;

		/** Height or Time Based Activations **/
		nLastPOWBlock = 56700;
		nModifierUpdateBlock = 615800;
		nZerocoinStartHeight = 863787;
		nZerocoinStartTime = 1533945600; // October 17, 2017 4:30:00 AM
		nBlockEnforceSerialRange = 895400; //Enforce serial range starting this block
		nBlockRecalculateAccumulators = 908000; //Trigger a recalculation of accumulators
		nBlockFirstFraudulent = 891737; //First block that bad serials emerged
		nBlockLastGoodCheckpoint = 891730; //Last valid accumulator checkpoint
		nBlockEnforceInvalidUTXO = 902850; //Start enforcing the invalid UTXO's

		/**
		* Build the genesis block. Note that the output of the genesis coinbase cannot
		* be spent as it did not originally exist in the database.
		*
		* CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
		*   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
		*     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
		*     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
		*   vMerkleTree: e0028e
		*/
		const char* pszTimestamp = "March 2, 2018 - East And West, Both Coasts Brace For Major Winter Storms";
		CMutableTransaction txNew;
		txNew.vin.resize(1);
		txNew.vout.resize(1);
		txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
		txNew.vout[0].nValue = 8000250 * COIN;
		txNew.vout[0].scriptPubKey = CScript() << ParseHex("04bcc3ef3417ba00ab55e3de807a776ade43cbd634a7e2cff383fecc6920cf918b2ad427f6b0a3f8d38f5a41d5dcbf35b394521bd08fcb5f40749df5bfe7d42fe2") << OP_CHECKSIG;
		genesis.vtx.push_back(txNew);
		genesis.hashPrevBlock = 0;
		genesis.hashMerkleRoot = genesis.BuildMerkleTree();
		genesis.nVersion = 1;
		genesis.nTime = 1520007171;
		genesis.nBits = 0x1e0ffff0;
		genesis.nNonce = 416509442;
		genesis.payee = txNew.vout[0].scriptPubKey;

		/*
		hashGenesisBlock = uint256("0x01");
		if (true && genesis.GetHash() != hashGenesisBlock)
		{
		//genesis.nNonce = 1000000;
		printf("recalculating params for mainnet.\n");
		printf("old mainnet genesis nonce: %d\n", genesis.nNonce);
		printf("old mainnet genesis hash:  %s\n", hashGenesisBlock.ToString().c_str());
		// deliberately empty for loop finds nonce value.
		for(genesis.nNonce == 0; !CheckProofOfWorkGen(genesis.GetHash(),genesis.nBits); genesis.nNonce++){ }
		printf("new mainnet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
		printf("new mainnet genesis nonce: %d\n", genesis.nNonce);
		printf("new mainnet genesis hash: %s\n", genesis.GetHash().ToString().c_str());
		}
		*/

		hashGenesisBlock = genesis.GetHash();
		assert(hashGenesisBlock == uint256("0x000008ae5cfd29b6680b53c7061840fdeea45259bd5320322a541a0e4305728a"));
		assert(genesis.hashMerkleRoot == uint256("0xb68f3b6cefa827045e8bac505203050c9d247c10d7fe2a951575924427a51052"));

		//vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "divi.seed.fuzzbawls.pw"));     // Primary DNS Seeder from Fuzzbawls
		//vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "divi.seed2.fuzzbawls.pw"));    // Secondary DNS Seeder from Fuzzbawls
		//vSeeds.push_back(CDNSSeedData("coin-server.com", "coin-server.com"));         // Single node address
		//vSeeds.push_back(CDNSSeedData("s3v3nh4cks.ddns.net", "s3v3nh4cks.ddns.net")); // Single node address
		//vSeeds.push_back(CDNSSeedData("178.254.23.111", "178.254.23.111"));           // Single node address
		vFixedSeeds.clear();
		vSeeds.clear();

		base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 30);
		base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 13);
		base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 212);
		base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
		base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
		// 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
		base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

		convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

		fRequireRPCPassword = true;
		fMiningRequiresPeers = false;
		fAllowMinDifficultyBlocks = false;
		fDefaultConsistencyChecks = false;
		fRequireStandard = true;
		fMineBlocksOnDemand = false;
		fSkipProofOfWorkCheck = false;
		fTestnetToBeDeprecatedFieldRPC = false;
		fHeadersFirstSyncingActive = false;

		nPoolMaxTransactions = 3;
		strSporkKey = "04B433E6598390C992F4F022F20D3B4CBBE691652EE7C48243B81701CBDB7CC7D7BF0EE09E154E6FCBF2043D65AF4E9E97B89B5DBAF830D83B9B7F469A6C45A717";
		strObfuscationPoolDummyAddress = "D87q2gC9j6nNrnzCsg4aY6bHMLsT9nUhEw";
		nStartMasternodePayments = 1533945600; //Wed, 11 Aug 2018 00:00:00 GMT

											   /** Zerocoin */
		zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
			"4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
			"6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
			"7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
			"8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
			"31438167899885040445364023527381951378636564391212010397122822120720357";
		nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
		nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
		nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
		nRequiredAccumulation = 2;
		nDefaultSecurityLevel = 100; //full security level for accumulators
		nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
		nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
	}

	const Checkpoints::CCheckpointData& Checkpoints() const
	{
		return data;
	}
};
static CBetaParams betaParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
		pchMessageStart[0] = 0xdf;
		pchMessageStart[1] = 0xa0;
		pchMessageStart[2] = 0x8d;
		pchMessageStart[3] = 0x8b;
		vAlertPubKey = ParseHex("04b6f1de3a2f94bfc70917cd7e14a948e78c1638b30524370ac40d3a0fa298e10bbaf7f5c564279c3fddf3f9f785cf3ac4e2031a5e6451f455e5aa60d1e536b379");
        nDefaultPort = 51474;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // DIVI: 1 day
        nTargetSpacing = 1 * 60;  // DIVI: 1 minute
        nLastPOWBlock = 56250;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 43199500 * COIN;
        nZerocoinStartHeight = 201576;
        nZerocoinStartTime = 1501776000;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 9908000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 9891737; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 9891730; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 9902850; //Start enforcing the invalid UTXO's

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1520007171;
        genesis.nNonce = 416509442;
        
		pchMessageStart[0] = 0xdf;
		pchMessageStart[1] = 0xa0;
		pchMessageStart[2] = 0x8d;
		pchMessageStart[3] = 0x78;
		vAlertPubKey = ParseHex("046e70d194b1b6b63b9c5431ea83c7b17d0db8930408b1e7937e41759a799e8fcd22d99ffc0c880094bb07a852a9020f810068417e65d19def8ffbdfa90727b637");
		nDefaultPort = 51472;
		bnProofOfWorkLimit = ~uint256(0) >> 20; // DIVI starting difficulty is 1 / 2^12
		nSubsidyHalvingInterval = 210000;
		nMaxReorganizationDepth = 100;
		nEnforceBlockUpgradeMajority = 750;
		nRejectBlockOutdatedMajority = 950;
		nToCheckBlockUpgradeMajority = 1000;
		nMinerThreads = 0;
		nTargetTimespan = 1 * 60; // DIVI: 1 day
		nTargetSpacing = 1 * 60;  // DIVI: 1 minute
		nMaturity = 1; //byrdset from 100
		nMasternodeCountDrift = 20;
		nMaxMoneyOut = 2535000000 * COIN;

		/** Height or Time Based Activations **/
		nLastPOWBlock = 2000000;
		nModifierUpdateBlock = 615800;
		nZerocoinStartHeight = 863787;
		nZerocoinStartTime = 1533945600; // October 17, 2017 4:30:00 AM
		nBlockEnforceSerialRange = 895400; //Enforce serial range starting this block
		nBlockRecalculateAccumulators = 908000; //Trigger a recalculation of accumulators
		nBlockFirstFraudulent = 891737; //First block that bad serials emerged
		nBlockLastGoodCheckpoint = 891730; //Last valid accumulator checkpoint
		nBlockEnforceInvalidUTXO = 902850; //Start enforcing the invalid UTXO's 
										   /*
        hashGenesisBlock = uint256("0x01");
       if (true && genesis.GetHash() != hashGenesisBlock)
        {
            printf("recalculating params for testnet.\n");
            printf("old testnet genesis nonce: %d\n", genesis.nNonce);
            printf("old testnet genesis hash:  %s\n", hashGenesisBlock.ToString().c_str());
            // deliberately empty for loop finds nonce value.
            for(genesis.nNonce == 0; genesis.GetHash() > bnProofOfWorkLimit; genesis.nNonce++){ } 
            printf("new testnet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
            printf("new testnet genesis nonce: %d\n", genesis.nNonce);
            printf("new testnet genesis hash: %s\n", genesis.GetHash().ToString().c_str());
        }
        */

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x000008ae5cfd29b6680b53c7061840fdeea45259bd5320322a541a0e4305728a"));

        vFixedSeeds.clear();
        vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "divi-testnet.seed.fuzzbawls.pw"));
        //vSeeds.push_back(CDNSSeedData("fuzzbawls.pw", "divi-testnet.seed2.fuzzbawls.pw"));
        //vSeeds.push_back(CDNSSeedData("s3v3nh4cks.ddns.net", "s3v3nh4cks.ddns.net"));
        //vSeeds.push_back(CDNSSeedData("88.198.192.110", "88.198.192.110"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet divi addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet divi script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet divi BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet divi BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet divi BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04348C2F50F90267E64FACC65BFDC9D0EB147D090872FB97ABAE92E9A36E6CA60983E28E741F8E7277B11A7479B626AC115BA31463AC48178A5075C5A9319D4A38";
        strObfuscationPoolDummyAddress = "y57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";
        nStartMasternodePayments = 1533945600; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Divi: 1 day
        nTargetSpacing = 1 * 60;        // Divi: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1520007171;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 12347;
        
        /*
        hashGenesisBlock = uint256("0x01");
        if (true && genesis.GetHash() != hashGenesisBlock)
        {
            printf("recalculating params for regtest.\n");
            printf("old regtest genesis nonce: %d\n", genesis.nNonce);
            printf("old regtest genesis hash:  %s\n", hashGenesisBlock.ToString().c_str());
            // deliberately empty for loop finds nonce value.
            for(genesis.nNonce == 0; genesis.GetHash() > bnProofOfWorkLimit; genesis.nNonce++){ } 
            printf("new regtest genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
            printf("new regtest genesis nonce: %d\n", genesis.nNonce);
            printf("new regtest genesis hash: %s\n", genesis.GetHash().ToString().c_str());
        }
        */
        
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 51476;
        
        assert(hashGenesisBlock == uint256("0x0eddf3476f5a5fc8404a7ec38a8dab5bd7e7cf6c6e057f7e38ec02ac45ae1b31"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
	case CBaseChainParams::UNITTEST:
		return unitTestParams;
	case CBaseChainParams::BETATEST:
		return betaParams;
	default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
