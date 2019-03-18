__Useful links__
- [docs](https://developers.eos.io/eosio-home/docs)
- [eosio-nodeos reference](https://developers.eos.io/eosio-nodeos/reference)

## Get chain id
```js
npm i request

const request = require('request')
request('https://eosbp.atticlab.net/v1/chain/get_info', function (error, response, body) {
  console.log('chain id:', JSON.parse(body).chain_id);
});
```

## Get key pair
```js
npm i eosjs-ecc

const ecc = require('eosjs-ecc')
ecc.randomKey().then(privateKey => {
console.log('Private Key:\t', privateKey) // wif
console.log('Public Key:\t', ecc.privateToPublic(privateKey)) // EOSkey...
})
```

**===================================================================================**
## Install and create a docker network
```sh
docker system prune -a && docker pull eosio/eos && docker network create eosnetwork
```


## Run the containers
```sh
docker run --name server --network=eosnetwork --rm -p 7777:7777 -i eosio/eos /bin/bash -c "nodeos -e -p eosio --plugin eosio::producer_plugin --plugin eosio::chain_api_plugin --plugin eosio::http_plugin --plugin eosio::history_api_plugin -d /mnt/dev/data --http-server-address=0.0.0.0:7777 --access-control-allow-origin=* --http-validate-host=0"

docker run --name wallet --network=eosnetwork --rm -p 5555:5555 -i eosio/eos /bin/bash -c "keosd --http-server-address=0.0.0.0:5555 --http-validate-host=0"
```
## Start
```sh
docker run --name tools --network=eosnetwork --rm -it eosio/eos /bin/bash

apt-get update && apt-get upgrade -y && apt-get install wget -y && apt-get install vim -y && apt-get install git -y && wget https://github.com/eosio/eosio.cdt/releases/download/v1.5.0/eosio.cdt_1.5.0-1_amd64.deb && apt install ./eosio.cdt_1.5.0-1_amd64.deb && cd contracts/hello/ && rm -rf * && vim hello.cpp
```
__Paste this__ and :wq!
```cpp
#include <eosiolib/eosio.hpp>

using namespace eosio;

class [[eosio::contract("hello")]] hello : public contract {
  public:
      using contract::contract;

      [[eosio::action]]
      void hi( name user ) {
         print( "Hello, ", user);
      }
};
EOSIO_DISPATCH( hello, (hi))
```
```sh
eosio-cpp -o hello.wasm hello.cpp --abigen && alias cleos="cleos --wallet-url http://wallet:5555 -u http://server:7777"
```

## Wallet
[Wallet tutorial](https://developers.eos.io/eosio-home/docs/wallets)
```sh
cleos wallet create --to-console && cleos wallet open && cleos wallet unlock
```

## Create keypairs
```sh
cleos create key --to-console
```

## Import the private key of the system account into the wallet and generated before
```sh
cleos wallet import --private-key="5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3"

cleos wallet import
private key
```

## Check wallet
```sh
cleos wallet keys
cleos wallet list
cleos wallet private_keys -n 
```

## Hello world contract

__Create an account for the contract__
```sh
cd /contracts/hello && cleos create account eosio hello {public key} -p eosio@active
```

__Create accounts__
```sh
cleos create account eosio bob {public key} 
cleos create account eosio alice {public key}
```

__Broadcast the compiled wasm to the blockchain__
```sh
cleos set contract hello /contracts/hello -p hello@active
```

__Now the contract is set, push an action to it.__
```sh
cleos push action hello hi '["bob"]' -p bob@active
```

## Deploy, Issue and Transfer Tokens
```sh
cd /contracts/ && git clone https://github.com/EOSIO/eosio.contracts --branch v1.4.0 --single-branch && cd eosio.contracts/eosio.token
```

__Create Account for Contract__
```sh
cleos create account eosio eosio.token EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
```

__Compile the Contract__
```sh
eosio-cpp -I include -o eosio.token.wasm src/eosio.token.cpp --abigen
```

__Deploy the Token Contract__
```sh
cleos set contract eosio.token . --abi eosio.token.abi -p eosio.token@active
```

__Create the Token__
```sh
cleos push action eosio.token create '[ "eosio", "1000000000.0000 SYS"]' -p eosio.token@active
```

__Issue Tokens__
```sh
cleos push action eosio.token issue '[ "alice", "100.0000 SYS", "memo" ]' -p eosio@active
```

__Transfer Tokens__
```sh
cleos push action eosio.token transfer '[ "alice", "bob", "25.0000 SYS", "m" ]' -p alice@active
```

__Get balance__
```sh
cleos get currency balance eosio.token bob SYS
cleos get currency balance eosio.token alice SYS
```

## Get account
```sh
cleos get account {account name}
```

## Get all block producers
```sh
unalias cleos
cleos --url https://eosbp.atticlab.net system listproducers
cleos --url https://eosbp.atticlab.net system listproducers -L eoswingdotio
```

## Find the formula for calculating the vote decay for the account

From [here](https://www.eoscanada.com/en/how-is-your-vote-strength-calculated-on-eos)
```cpp
   double stake2vote( int64_t staked ) {
      /// TODO subtract 2080 brings the large numbers closer to this decade
      double weight = int64_t( (now() - (block_timestamp::block_timestamp_epoch / 1000)) / (seconds_per_day * 7) )  / double( 52 );
      return double(staked) * std::pow( 2, weight );
   }
```

## Vote for a pair of BP

__Using Scatter with imported private key:__
```5JM8Kc58tRwtCif5b4sh6kETs98HaXeXhH5eaUmQaua72pFfSy4```
- Voting on [jungletestnet](https://monitor.jungletestnet.io/)
__Account:__
- dominatorss1

__Voting result you may see on [jungle.bloks](https://jungle.bloks.io/account/dominatorss1)__<br><br>
__Scatter mnemonic__
- weird garment oblige private holiday joy medal athlete occur army manual firm

## Create another permission for your own one
```sh
cleos set account permission bob bob1 EOS8FtkJYmJZWx37e6bLmqgi6urptxHNjLdyqG63ife1ToDYyN31f -p bob@owner
```

## Create Block Producer
```sh
cleos system regproducer bob EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV http://producer.site 25
```

## Find the top 10 largest proxies
[Here](https://www.alohaeos.com/vote/proxy?sort=rank&sortDir=asc)


## Calculate how much EOS gets per day top 3 BP

Just a sum of first three BP`s on this [site](https://eosauthority.com/producers_rank)
