#include <eosiolib/eosio.hpp>
#include <map>

using namespace eosio;
class [[eosio::contract("spore")]] spore : public eosio::contract {

        public:
            using contract::contract;

            spore(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds), coordinates(_self, _self.value){}

            [[eosio::action]]
            void createcoor(name author, const uint32_t id, const uint32_t x, const uint32_t y)
            {

                // create corresponding starting coordinate on screen for entity
                coordinates.emplace(author, [&](auto& new_coordinate) {
                    new_coordinate.id = id;
                    //new_coordinate.entityId = id;
                    new_coordinate.x = 0;
                    new_coordinate.y = 0;
                });

                print("createEntity: entity #", id, " created");
            }

            [[eosio::action]]
            void killcoor(name author, const uint32_t id){
                auto coordinates_lookup = coordinates.find(id);
                coordinates.erase(coordinates_lookup);
                print("killEntity: coordinate#", id, " destroyed");
            }

            [[eosio::action]]
            void coordexists(const uint32_t id) {
                auto coordinate_lookup = coordinates.find(id);
                eosio_assert(coordinate_lookup != coordinates.end(), 
                    "coordinateexists: Coordinate does not exist");
            }

            private:
            // entity can be either food or spore
            /*enum EntityType{
                Food, Spore
            };*/
            // @abi table Spore i64
            struct [[eosio::table]] Coordinate {
                uint64_t id;
                //uint64_t entityId;
                uint64_t x;
                uint64_t y;

                uint64_t primary_key() const { return id; }
                EOSLIB_SERIALIZE(Coordinate, (id)(x)(y) )
            };
            typedef multi_index<"coordinates"_n, Coordinate> coordinate_table;
            coordinate_table coordinates;
    };

    EOSIO_DISPATCH( spore, (createcoor)(killcoor)(coordexists) )