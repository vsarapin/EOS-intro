## Install and create a docker network
```sh
docker system prune -a && docker pull eosio/eos && docker network create eosnetwork
```


## Run the containers
```sh
docker run --name server --network=eosnetwork --rm -p 8888:8888 -i eosio/eos /bin/bash -c "nodeos -e -p eosio --plugin eosio::producer_plugin --plugin eosio::chain_api_plugin --plugin eosio::http_plugin --plugin eosio::history_api_plugin -d /mnt/dev/data --http-server-address=0.0.0.0:8888 --access-control-allow-origin=* --http-validate-host=0"

docker run --name wallet --network=eosnetwork --rm -p 5555:5555 -i eosio/eos /bin/bash -c "keosd --http-server-address=0.0.0.0:5555 --http-validate-host=0"
```
## Start
```sh
docker run --name tools --network=eosnetwork --rm -it eosio/eos /bin/bash

apt-get update && apt-get upgrade -y && apt-get install wget -y &&  wget https://github.com/eosio/eosio.cdt/releases/download/v1.5.0/eosio.cdt_1.5.0-1_amd64.deb && apt install ./eosio.cdt_1.5.0-1_amd64.deb && alias cleos="cleos --wallet-url http://wallet:5555 -u http://server:8888" && mkdir eos-contract && cd eos-contract && mkdir simplecontract && cd simplecontract && mkdir contracts && cd contracts && mkdir spore && alias cleos="cleos --wallet-url http://wallet:5555 -u http://server:8888"
```
## Copy contract to docker and compile it
```sh
docker cp spore.cpp tools:/eos-contract/simplecontract/contracts

eosio-cpp -o spore.wasm spore.cpp --abigen && mv spore.wasm spore && mv spore.abi spore
```
## Create new wallet with the name "vsarapin"
```
cleos wallet create --to-console -n vsarapin && cleos wallet open -n vsarapin && cleos wallet unlock -n vsarapin
```

## Generate two pair of keys
```sh
cleos create key --to-console
```

## Import the generated private keys in the wallet(you need to specify the wallet at the end)
```sh
cleos wallet import -n vsarapin --private-key="5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3"
cleos wallet import -n vsarapin --private-key={private_key}
```

## Deploy to the single node test net
```sh
cleos create account eosio spore.ctr [public key] [public key]
cleos set contract spore.ctr ./spore -p spore.ctr
cleos create account eosio tester [public key] [public key]
```

## Try
```sh
cleos push action spore.ctr createentity '["tester", 0, "blue", 0]' -p tester

cleos push action spore.ctr moveentity '["tester", 0, 10, 10]' -p tester

cleos push action spore.ctr killentity '["tester", 0, 0]' -p tester
```