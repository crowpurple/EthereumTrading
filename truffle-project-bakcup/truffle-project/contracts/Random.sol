pragma solidity ^0.4.0;

contract Random
{
    uint flag;
    
    function getNumber() constant returns (uint)
    {
		return flag;
    }
    
    function getBlocknumber() constant returns (uint)
    {
        return block.number;
    }
    
    function getBlockdifficulty() constant returns (uint)
    {
        return block.difficulty;
    }
    
    function getBlockgaslimit() constant returns (uint)
    {
        return block.gaslimit;
    }
    
    function getBlocktimestamp() constant returns (uint)
    {
        return block.timestamp;
    }
    
    function getTxgasprice() constant returns (uint)
    {
        return tx.gasprice;
    }
    
    function setNumber(uint fa) payable
    {
	flag=(flag+block.number%10+block.difficulty%10+block.gaslimit%10+block.timestamp%10+tx.gasprice%10+fa)%10;
    }
}

