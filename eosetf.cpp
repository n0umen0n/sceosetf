#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <cmath>
#include <eosio/singleton.hpp>

//REQUIRE AUTH IGALE POOLE
using namespace eosio;
using namespace std;

class [[eosio::contract("eosetf")]] eosetf : public contract {

public:
	using contract::contract;


  struct [[eosio::table]] account {
         asset    balance;

         uint64_t primary_key()const { return balance.symbol.code().raw(); }
      };
      typedef eosio::multi_index< name("accounts"), account > accounts;

      
      
struct [[eosio::table]] currency_stats {
            asset    supply;
            asset    max_supply;
            name     issuer;

            uint64_t primary_key()const { return supply.symbol.code().raw(); }
         };

         typedef eosio::multi_index< "stat"_n, currency_stats > stats;


struct [[eosio::table]] etfinf {
         asset    token;
         uint64_t ratio;
         uint64_t maxamount;
         uint64_t minamount;
         uint64_t multiplier;

         uint64_t primary_key()const { return token.symbol.code().raw(); }
      };
            typedef eosio::multi_index< name("etfinfo"), etfinf > etfinfo;



struct [[eosio::table]] useritokens {
         asset    token;
         uint64_t ratio;
         uint64_t multiplier;

         

         uint64_t primary_key()const { return token.symbol.code().raw(); }
      };

      typedef eosio::multi_index< name("userstokens"), useritokens > useritokenid;



struct [[eosio::table]] refundstab {
         asset    token;
         name contract;
         
         uint64_t primary_key()const { return token.symbol.code().raw(); }
      };

      typedef eosio::multi_index< name("refundstab"), refundstab > refundstable;




TABLE pausetabla{

  bool ispaused;
};
typedef eosio::singleton<"pauze"_n, pausetabla> pausetab;


TABLE etfsize{

  int8_t size;
};
typedef eosio::singleton<"etfsize"_n, etfsize> etfsizetab;


TABLE basetoken{

  symbol base;
};
typedef eosio::singleton<"basetoken"_n, basetoken> basetoktab;


/*

[[eosio::action]]
void delusertok(name user)
{

require_auth( _self );

useritokenid input(get_self(), user.value);

for (auto iter = input.begin(); iter != input.end();)
{

input.erase(iter++);

}
}
*/
/*
[[eosio::action]]
void testquery(asset value)
{

accounts _newdexpublic("consortiumlv"_n , "consortiumlv"_n.value );

auto pede = _newdexpublic.find(value.symbol.code().raw());

//check(false, "You have already voted in this poll.");

check(false, pede->balance.amount);


}

*/

[[eosio::action]]
void setbasetok(symbol base)
{

  require_auth( _self );

basetoktab basetable(_self, _self.value);
  basetoken soloiter;

  if(!basetable.exists()){
    basetable.set(soloiter, _self);
  }
  else{
    soloiter = basetable.get();
  }
  soloiter.base = base;
  basetable.set(soloiter, _self);
}


[[eosio::action]]
void setsize(int8_t size)
{

  require_auth( _self );

etfsizetab sizetable(_self, _self.value);
  etfsize soloiter;
  if(!sizetable.exists()){
    sizetable.set(soloiter, _self);
  }
  else{
    soloiter = sizetable.get();
  }
  soloiter.size = size;
  sizetable.set(soloiter, _self);
}



[[eosio::action]]
void pause(bool ispaused)
{

  require_auth( _self );

pausetab pausetable(_self, _self.value);
  pausetabla soloiter;
  if(!pausetable.exists()){
    pausetable.set(soloiter, _self);
  }
  else{
    soloiter = pausetable.get();
  }
  soloiter.ispaused = ispaused;
  pausetable.set(soloiter, _self);
}





[[eosio::action]]
void insertrefund( asset token, name contract )
{

  require_auth( _self );

     auto sym = token.symbol;
    refundstable reftab(get_self(), _self.value);
    auto existing = reftab.find( sym.code().raw() );
    if(existing==reftab.end() ) {
    reftab.emplace( _self, [&]( auto& s ) {
       s.contract    = contract;
       s.token        = token;
    });
}

         else{
           //auto newinput = input.find( quantity.symbol.code().raw() );
            reftab.modify(existing,name("cet.f"), [&]( auto& s ){
            s.contract    = contract;
            s.token        = token;
        });
      }
}


[[eosio::action]]
void insertratio( const uint64_t&   ratio,
                    const asset&  token, const uint64_t&   minamount, const uint64_t&   maxamount, const uint64_t&   multiplier )
{


  require_auth( _self );


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
            rattab.modify(existing,name("cet.f"), [&]( auto& s ){
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

    stats statstable( _self, sym.code().raw() );
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

    stats statstable( _self, sym.code().raw() );
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
   stats statstable( _self, sym.raw() );
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

check(false, "This action will be activated when CETF distribution ends or latest on 31.04.2021");
        
        refund_tokens_back (from, to, quantity, memo);
        
        

     sub_balance( from, quantity );
   add_balance( to, quantity, payer );

   retire (quantity,memo);

  }
  else{
     sub_balance( from, quantity );
   add_balance( to, quantity, payer );
  }
   // default transfer
   
}






//UUS
[[eosio::on_notify("dappservices::transfer")]]
void issueetfdapp (name from, name to, asset quantity, const string memo)

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

[[eosio::on_notify("pizzatotoken::transfer")]]
void issueetfpizza (name from, name to, asset quantity, std::string memo){
     
 savetokens(from, quantity,to);

}

[[eosio::on_notify("chexchexchex::transfer")]]
void issueetfchex (name from, name to, asset quantity, std::string memo){
     
 savetokens(from, quantity,to);

}


[[eosio::on_notify("newdexissuer::transfer")]]
void issueetfndx (name from, name to, asset quantity, std::string memo){
     
 savetokens(from, quantity,to);

}

[[eosio::on_notify("eosiotptoken::transfer")]]
void issueetftpt (name from, name to, asset quantity, std::string memo){
     
 savetokens(from, quantity,to);

}

[[eosio::on_notify("emanateoneos::transfer")]]
void issueetfemt (name from, name to, asset quantity, std::string memo){
     
 savetokens(from, quantity,to);

}

[[eosio::on_notify("minedfstoken::transfer")]]
void issueetfdfs (name from, name to, asset quantity, std::string memo){
     
 savetokens(from, quantity,to);

}



private:

    void refund_tokens_back(name from, name to, asset quantity, std::string memo) {

    check (quantity.symbol == symbol("EOSETF", 4), "Token with wrong symbol.");

    check (quantity.amount >= 10000, "Can't redeem less than 1 EOSETF" );

    check (quantity.amount <= 2000000, "Can't more than 200 EOSETF" );

    pauseornot();

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

    stats statstable( _self, sym.code().raw() );
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

        stats statstable( _self, sym_code_raw );
        auto existing = statstable.find( sym_code_raw );
        check( existing != statstable.end(), "Token with that symbol name does not exist - Please create the token before issuing" );

        const auto& st = *existing;
        
        check( quantity.is_valid(), "Invalid quantity value" );
        check( st.supply.symbol == quantity.symbol, "Symbol precision mismatch" );
        check( st.max_supply.amount - st.supply.amount >= quantity.amount, "Quantity value cannot exceed the available supply" );

        statstable.modify( st, name("cet.f"), [&]( auto& s ) {
            s.supply += quantity;
        });
        
        
      
       add_balance( owner, quantity, name("cet.f"));

       //add_balance( owner, quantity, owner);
    }


void pauseornot( ) {

pausetab pauztab(_self, _self.value);
pausetabla iter;

iter = pauztab.get();

check(iter.ispaused, "Creation and redemption is currently halted.");
}








void savetokens( name from, asset quantity, name to )
    {
if (to  != "cet.f"_n) return;

pauseornot();

   etfinfo sinput(get_self(), _self.value);
      auto secinput = sinput.find( quantity.symbol.code().raw() );


    useritokenid input(get_self(), from.value);
      auto newinput = input.find( quantity.symbol.code().raw() );

 check (quantity.symbol == secinput->token.symbol, "Incorrect symbol.");

 check (quantity.amount <= secinput->maxamount, "Maximum creation threshold is 200 EOSETF.");

 check (quantity.amount >= secinput->minamount, "Minimum creation threshold is 1 EOSETF.");


     if( newinput == input.end() ) {
         input.emplace( name("cet.f"), [&]( auto& a ){
            a.token= quantity;
            a.ratio = secinput->ratio;
            a.multiplier= secinput->multiplier;
         });

     }
         else{
            input.modify(newinput,name("cet.f"), [&]( auto& a ){
            a.token += quantity;
            a.ratio = secinput->ratio;
            a.multiplier= secinput->multiplier;
        });
      }

checkratuus (from);
}


void checkratuus( name from )
    {

basetoktab basetable(_self, _self.value);
basetoken baseiter;

baseiter = basetable.get();    

    
useritokenid input(get_self(), from.value);

auto size = std::distance(input.cbegin(),input.cend());

etfsizetab sizetable(_self, _self.value);
etfsize soloiter;

soloiter = sizetable.get();

//if size is smaller will not issue CETF/EOSETF, just saves the token values in the table. 
if (size == soloiter.size ) {

const auto& basetokrow = input.find(baseiter.base.code().raw() );

for (auto iter = input.begin(); iter != input.end();)
{

check (iter->token.amount != 0, "Doggy Afuera!");

check ((iter->token.amount * iter->multiplier / basetokrow->token.amount == iter->ratio), "Incorrect token ratios.");


input.erase(iter++);

}


/*
//CHANGE IF BASETOKROW CHANGES FROM DFS
struct asset numberofetfs = {int64_t ((basetokrow->token.amount/361)*10000), symbol ("EOSETF", 4)};

createetf(from, numberofetfs );

auto sym = symbol ("ETFF", 4);


stats statstable( _self, sym.code().raw() );
auto existing = statstable.find( sym.code().raw() );
const auto& st = *existing;

if (st.supply.amount < 500000000000)

{

const int64_t interval = (100000000000);  

int64_t halvings =  (st.supply.amount / interval);

int64_t rewardint =  (300000000);

int64_t divider = pow( 2 , halvings);

int64_t adjrewardint = rewardint/divider;

struct asset reward = {int64_t (adjrewardint*numberofetfs.amount/10000), symbol ("ETFF", 4)};

createetf(from, reward );

}
}
}
*/

//CHANGE IF BASETOKROW CHANGES FROM DFS
struct asset numberofetfs = {int64_t ((basetokrow->token.amount/140)*10000), symbol ("EOSETF", 4)};

createetf(from, numberofetfs );

auto sym = symbol ("CETF", 4);


stats statstable( _self, sym.code().raw() );
auto existing = statstable.find( sym.code().raw() );
const auto& st = *existing;

if (st.supply.amount < 800000000000)

{

const int64_t interval = (200000000000);  

int64_t halvings =  (st.supply.amount / interval);

int64_t rewardint =  (650000000);

int64_t divider = pow( 2 , halvings);

int64_t adjrewardint = rewardint/divider;

struct asset reward = {int64_t (adjrewardint*numberofetfs.amount/10000), symbol ("CETF", 4)};

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
