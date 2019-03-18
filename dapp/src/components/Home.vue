<template>
    <v-card>
        <v-card-title>
            Explorer
            <v-spacer></v-spacer>
            <v-text-field
                    v-model="search"
                    append-icon="search"
                    label="Search"
                    single-line
                    hide-details
            ></v-text-field>
        </v-card-title>
        <v-data-table
                :headers="headers"
                :items="producers"
                :search="search"
        >
            <template v-slot:items="props">
                <td>{{ props.item.index }}</td>
                <td class="text-xs-right">{{ props.item.owner }}</td>
                <td class="text-xs-right">{{ effectiveVotes(props.item.total_votes) }}</td>
                <td class="text-xs-right">{{ votePercent(props.item.total_votes) }}</td>
            </template>
            <v-alert v-slot:no-results :value="true" color="error" icon="warning">
                Your search for "{{ search }}" found no results.
            </v-alert>
        </v-data-table>
    </v-card>
</template>

<script>
    import EosApi from 'eosjs-api'

    const options = {
        httpEndpoint: 'https://eosbp.atticlab.net', // default, null for cold-storage
        fetchConfiguration: {}
    };
    const eos = EosApi(options);

    export default {
        name: 'Home',
        data: function () {
            return {
                search: '',
                total_producer_vote_weight: '',
                headers: [
                    {
                        text: 'Rank',
                        align: 'left',
                        sortable: true,
                        value: 'index'
                    },
                    {text: 'Name', align: 'right', value: 'owner'},
                    {text: 'Votes in Effect', align: 'right', value: 'total_votes'},
                    {text: 'Votes %', align: 'right', value: 'total_votes'},
                ],
                producers: []
            }
        },
        methods: {
            votePercent(num) {
                let result = (num / this.total_producer_vote_weight * 100).toString();
                return result.slice(0, 5);
            },
            calculateVoteWeight() {

                //time epoch:
                //https://github.com/EOSIO/eos/blob/master/contracts/eosiolib/time.hpp#L160

                //stake to vote
                //https://github.com/EOSIO/eos/blob/master/contracts/eosio.system/voting.cpp#L105-L109

                let timestamp_epoch = 946684800000;
                let dates_ = (Date.now() / 1000) - (timestamp_epoch / 1000);
                let weight_ = Math.floor(dates_ / (86400 * 7)) / 52;  //86400 = seconds per day 24*3600
                return Math.pow(2, weight_);
            },
            effectiveVotes(total_votes) {
                return (total_votes / this.calculateVoteWeight() / 10000).toFixed(0);
            },
        },
        created() {
            eos.getProducers(true).then(
                result => {
                    this.total_producer_vote_weight = result.total_producer_vote_weight;
                    for (let i = 0; i < result.rows.length; i++) {
                        result.rows[i].index = i + 1;
                    }
                    this.producers = result.rows.sort(function (a, b) {
                        let keyA = a.index,
                            keyB = b.index;
                        if (keyA < keyB) return -1;
                        if (keyA > keyB) return 1;
                        return 0;
                    });
                })
                .catch(err => {console.log(err)})
            // cleos --url https://eosbp.atticlab.net --print-response system listproducers
            // cleos  --url https://eosbp.atticlab.net get account eoslaomaocom --json
            // сleos --url https://eosbp.atticlab.net get table eosio eosio producers -l 100
            // eos.getAccount("eoslaomaocom").then(result => console.log(result));


        }
    }
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>

</style>
