#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <cmath>

//REQUIRE AUTH IGALE POOLE
using namespace eosio;
using namespace std;

class [[eosio::contract("eosetf")]] example : public contract {

public:
	using contract::contract;


  struct [[eosio::table]] account {
         asset    balance;

         uint64_t primary_key()const { return balance.symbol.code().raw(); }
      };
      typedef eosio::multi_index< name("accounts"), account > accounts;

      
      
      struct [[eosio::table]] currency_statka {
         asset    supply;
         asset    max_supply;
         name     issuer;
         uint64_t transfer_fee_ratio;
         name fee_receiver;

         uint64_t primary_key()const { return supply.symbol.code().raw(); }
      };
      typedef eosio::multi_index< name("statnew"), currency_statka > statka;



struct [[eosio::table]] etfinf {
         asset    token;
         uint64_t ratio;
         uint64_t maxamount;
         uint64_t minamount;
         uint64_t multiplier;

         uint64_t primary_key()const { return token.symbol.code().raw(); }
      };
            typedef eosio::multi_index< name("etfinf"), etfinf > etfinfo;



struct [[eosio::table]] useritokens {
         asset    token;
         uint64_t ratio;
         uint64_t multiplier;

         

         uint64_t primary_key()const { return token.symbol.code().raw(); }
      };

      typedef eosio::multi_index< name("useritokens"), useritokens > useritokenid;



struct [[eosio::table]] refundstab {
         asset    token;
         name contract;
         
         uint64_t primary_key()const { return token.symbol.code().raw(); }
      };

      typedef eosio::multi_index< name("refundstab"), refundstab > refundstable;







[[eosio::action]]
void insertrefund( asset token, name contract )
{
     auto sym = token.symbol;
    refundstable reftab(get_self(), _self.value);
    auto existing = reftab.find( sym.code().raw() );
    if(existing==reftab.end() ) {
    reftab.emplace( _self, [&]( auto& s ) {
       //s.supply.symbol = maximum_supply.symbol;
       s.contract    = contract;
       s.token        = token;
    });
}

         else{
           //auto newinput = input.find( quantity.symbol.code().raw() );
            reftab.modify(existing,name("consortiumtt"), [&]( auto& s ){
            s.contract    = contract;
            s.token        = token;
        });
      }
}


[[eosio::action]]
void insertratio( const uint64_t&   ratio,
                    const asset&  token, const uint64_t&   minamount, const uint64_t&   maxamount, const uint64_t&   multiplier )
{
     auto sym = token.symbol;
    etfinfo rattab(get_self(), _self.value);
    auto existing = rattab.find( sym.code().raw() );
    if(existing==rattab.end() ) {
    rattab.emplace( _self, [&]( auto& s ) {
       //s.supply.symbol = maximum_supply.symbol;
       s.ratio    = ratio;
       s.token        = token;
       s.minamount    = minamount;
       s.maxamount        = maxamount;
       s.multiplier        =  multiplier;
    });
}

         else{
           //auto newinput = input.find( quantity.symbol.code().raw() );
            rattab.modify(existing,name("consortiumtt"), [&]( auto& s ){
            s.ratio    = ratio;
            s.token        = token;
            s.minamount    = minamount;
            s.maxamount        = maxamount;
            s.multiplier        =  multiplier;
        });
      }
}


[[eosio::action]]
void create( const name&   issuer,
                    const asset&  maximum_supply )
{
    require_auth( _self );

    auto sym = maximum_supply.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( maximum_supply.is_valid(), "invalid supply");
    check( maximum_supply.amount > 0, "max-supply must be positive");

    statka statstable( _self, sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing == statstable.end(), "token with symbol already exists" );

    statstable.emplace( _self, [&]( auto& s ) {
       s.supply.symbol = maximum_supply.symbol;
       s.max_supply    = maximum_supply;
       s.issuer        = issuer;
    });
}

[[eosio::action]]
void issue( const name& to, const asset& quantity, const string& memo )
{
    auto sym = quantity.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    statka statstable( _self, sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing != statstable.end(), "token with symbol does not exist, create token before issue" );
    const auto& st = *existing;
    check( to == st.issuer, "tokens can only be issued to issuer account" );

    require_auth( st.issuer );
    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must issue positive quantity" );

    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    check( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

    statstable.modify( st, same_payer, [&]( auto& s ) {
       s.supply += quantity;
    });

    add_balance( st.issuer, quantity, st.issuer );
}


[[eosio::action]]
void transfer( name    from,
                      name    to,
                      asset   quantity,
                      std::string  memo )
{
   check( from != to, "cannot transfer to self" );
   require_auth( from );
   check( is_account( to ), "to account does not exist");
   auto sym = quantity.symbol.code();
   statka statstable( _self, sym.raw() );
   const auto& st = statstable.get( sym.raw() );

   require_recipient( from );
   require_recipient( to );

   check( quantity.is_valid(), "invalid quantity" );
   check( quantity.amount > 0, "must transfer positive quantity" );
   check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
   check( memo.size() <= 256, "memo has more than 256 bytes" );

   auto payer = has_auth( to ) ? to : from;

   auto separator_pos = memo.find(':');


if ( to == get_self()){
        
        refund_tokens_back (from, to, quantity, memo);
        
        retire (quantity,memo);

         sub_balance( from, quantity );
   add_balance( to, quantity, payer );

  }
  else{
     sub_balance( from, quantity );
   add_balance( to, quantity, payer );
  }
   // default transfer
   
}






//UUS
[[eosio::on_notify("dappservices::transfer")]]
void testetfdapp (name from, name to, asset quantity, const string memo)

{   

savetokens(from, quantity,to);

}

[[eosio::on_notify("effecttokens::transfer")]]
void issueetfefx (name from, name to, asset quantity, std::string memo){
   

savetokens(from, quantity,to);

}


[[eosio::on_notify("core.ogx::transfer")]]
void issueetfogx (name from, name to, asset quantity, std::string memo){

savetokens(from, quantity,to);

}


[[eosio::on_notify("everipediaiq::transfer")]]
void issueetfiq (name from, name to, asset quantity, std::string memo){

savetokens(from, quantity,to);

}


[[eosio::on_notify("vig111111111::transfer")]]
void issueetfvig (name from, name to, asset quantity, std::string memo){

savetokens(from, quantity,to);

}


[[eosio::on_notify("token.defi::transfer")]]
void issueetfbox (name from, name to, asset quantity, std::string memo){
     
     savetokens(from, quantity,to);

}



[[eosio::on_notify("dadtoken1111::transfer")]]
void issueetfdad (name from, name to, asset quantity, std::string memo){
     
 savetokens(from, quantity,to);

}



private:

    void refund_tokens_back(name from, name to, asset quantity, std::string memo) {

    check (quantity.amount >= 10000, "Can't redeem less than 1 EOSETF" );

    refundstable reftab(get_self(), _self.value);

    for (auto iter = reftab.begin(); iter != reftab.end(); iter++)
{

     struct asset refundasset = {int64_t ((quantity.amount * iter->token.amount)/10000), iter->token.symbol};

     send_back(to, from, refundasset, memo, iter->contract);  

}

}



void sub_balance( name owner, asset value ) 
{
   accounts from_acnts( _self, owner.value );
   const auto& from = from_acnts.get( value.symbol.code().raw(), "no balance object found in accounts" );

   

      check( from.balance >= ( value ), "sub_balance: from.balance overdrawn balance" );
  

   from_acnts.modify( from, owner, [&]( auto& a ) {
         a.balance -= value;
   });
}


void add_balance( name owner, asset value, name ram_payer )
{
   accounts to_acnts( _self, owner.value );
   auto to = to_acnts.find( value.symbol.code().raw() );
   if( to == to_acnts.end() ) {
      to_acnts.emplace( ram_payer, [&]( auto& a ){
        a.balance = value;
      });

   } else {
      to_acnts.modify( to, same_payer, [&]( auto& a ) {
        a.balance += value;
      });
   }
}



void retire( asset quantity, std::string memo )
{
    auto sym = quantity.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    statka statstable( _self, sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing != statstable.end(), "token with symbol does not exist" );
    const auto& st = *existing;

    //require_auth( st.issuer );
    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must retire positive quantity" );

    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );

    statstable.modify( st, same_payer, [&]( auto& s ) {
       s.supply -= quantity;
    });

    sub_balance( st.issuer, quantity );
}



void createetf( name owner, asset quantity )
    {
        auto sym = quantity.symbol;
        check( sym.is_valid(), "Invalid symbol name" );

        auto sym_code_raw = sym.code().raw();

        statka statstable( _self, sym_code_raw );
        auto existing = statstable.find( sym_code_raw );
        check( existing != statstable.end(), "Token with that symbol name does not exist - Please create the token before issuing" );

        const auto& st = *existing;
        
        check( quantity.is_valid(), "Invalid quantity value" );
        check( st.supply.symbol == quantity.symbol, "Symbol precision mismatch" );
        check( st.max_supply.amount - st.supply.amount >= quantity.amount, "Quantity value cannot exceed the available supply" );

        statstable.modify( st, name("consortiumtt"), [&]( auto& s ) {
            s.supply += quantity;
        });
        
        
      
        add_balance( owner, quantity, name("consortiumtt"));
    }



void savetokens( name from, asset quantity, name to )
    {
if (to  != "consortiumtt"_n) return;

   etfinfo sinput(get_self(), _self.value);
      auto secinput = sinput.find( quantity.symbol.code().raw() );


    useritokenid input(get_self(), from.value);
      auto newinput = input.find( quantity.symbol.code().raw() );

 check (quantity.symbol == secinput->token.symbol, "Incorrect symbol.");

 check (quantity.amount <= secinput->maxamount, "Maximum creation thershold is 70 EOSETF.");

 check (quantity.amount >= secinput->minamount, "Minimum creation thershold is 1 EOSETF.");


     if( newinput == input.end() ) {
         input.emplace( name("consortiumtt"), [&]( auto& a ){
            a.token= quantity;
            a.ratio = secinput->ratio;
            a.multiplier= secinput->multiplier;
         });

     }
         else{
            input.modify(newinput,name("consortiumtt"), [&]( auto& a ){
            a.token += quantity;
            a.ratio = secinput->ratio;
            a.multiplier= secinput->multiplier;
        });
      }

checkratuus (from);
}


void checkratuus( name from )
    {

symbol sym = symbol("EFX", 4);
    
useritokenid input(get_self(), from.value);

auto size = std::distance(input.cbegin(),input.cend());

if (size == 7) {

const auto& efxrow = input.find(sym.code().raw() );

for (auto iter = input.begin(); iter != input.end();)
{

check (iter->token.amount != 0, "Doggy Afuera!");

check ((iter->token.amount * iter->multiplier / efxrow->token.amount == iter->ratio), "Incorrect token ratios.");

input.erase(iter++);

}


struct asset numberofetfs = {int64_t ((efxrow->token.amount/106593)*10000), symbol ("EOSETF", 4)};

createetf(from, numberofetfs );

auto sym = symbol ("ETFF", 4);

statka statstable( _self, sym.code().raw() );
auto existing = statstable.find( sym.code().raw() );
const auto& st = *existing;

if (st.supply.amount < 500000000000)

{

const int64_t interval = (100000000000);  

int64_t halvings =  (st.supply.amount / interval);

int64_t rewardint =  (120000000);

int64_t divider = pow( 2 , halvings);

int64_t adjrewardint = rewardint/divider;

struct asset reward = {int64_t (adjrewardint*numberofetfs.amount/10000), symbol ("ETFF", 4)};

createetf(from, reward );

}
}
}



 void send_back(name from, name to, asset quantity, std::string memo, name contract) {
    
      action(
      permission_level{get_self(),"active"_n},
      contract,
      "transfer"_n,
      std::make_tuple(from,to,quantity,memo)
    ).send();
  };










};
