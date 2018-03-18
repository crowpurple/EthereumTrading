pragma solidity ^0.4.0;

contract Purchase {
    uint public value;
    address public seller;
    address public buyer;
    enum State { Created, Locked, Inactive }
    State public state;

    function purchase(uint fa) payable
    {
	value=fa;
	seller=msg.sender;
	state=State.Created;
    }

    function getSomething() constant returns (address)
    {
	return seller;
    }

    function confirmPurchase() payable public
    {
	if(State.Locked == state && State.Inactive == state)
	{
		msg.sender.transfer(msg.value);
	}
	else{
		buyer = msg.sender;
	        state = State.Locked;
	}
    } 
    
    function confirmReceived() public
    {
	state = State.Inactive;
        seller.transfer(this.balance);
    }

    function refund() public
    {
	if(msg.sender == seller)
	{
		state==State.Created;
		buyer.transfer(this.balance);
	}	
    }
}
