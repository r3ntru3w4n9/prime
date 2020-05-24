/***********************************************************************

  FileName    [MasterCell.h]

  Author      [Yang, Chien Yi]

  This file defines the master cells and their elements, pins and
  blockages.

***********************************************************************/

#ifndef MASTERCELL_H
#define MASTERCELL_H

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <vector>
#include <assert.h>

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class MasterCellType;

class PinType
{
public:
    //Constructors
    PinType(const std::string&  PinName, int layer, MasterCellType& MCT):
    _PinName(PinName), _layer(layer), _MCT(MCT)
    { assert(layer >= 0); }

    PinType(PinType& a) :
    _PinName(a._PinName), _layer(a._layer), _MCT(a._MCT)
    { assert(a._layer >= 0); }

    //acceser
    std::string getPinName() const                                  { return _PinName; }
    int         getLayer() const                                    { return _layer; }

    //friend
    friend std::ostream& operator<<(std::ostream& os, const PinType& PT);

private:
    const std::string                   _PinName;
    const int                           _layer;
    const MasterCellType                _MCT;
};

std::ostream& operator<<(std::ostream& os, const PinType& PT)
{ os << "Pin Name : " << PT._PinName << " Layer : " << PT._layer << '\n'; return os; }

class BlockageType
{
public:
    //Constructors
    BlockageType(const std::string& BlkgName, int layer, int demand):
    _BlkgName(BlkgName), _layer(layer), _demand(demand)
    { assert(layer>=0 && demand>=0); }

    BlockageType(BlockageType& a):
    _BlkgName(a._BlkgName), _layer(a._layer), _demand(a._demand)
    { assert(a._layer>=0 && a._demand>=0); }

    //acceser
    std::string getBlkgName() const                                 { return _BlkgName; }
    int         getLayer() const                                    { return _layer; }
    int         getDemand() const                                   { return _demand; }

    //friend
    friend std::ostream& operator<<(std::ostream& os, const BlockageType& BT);

private:
    const std::string                   _BlkgName;
    const int                           _layer;
    const int                           _demand;
};

std::ostream& operator<<(std::ostream& os, const BlockageType& BT)
{ os << "Blockage Name : " << BT._PinName << " Layer : " << BT._layer << " Demand : " << BT._demand << '\n'; return os; }

class MasterCellType
{
public:
    //Constructor
    MasterCellType(const std::string& MCName, unsigned id): _MCName(MCName), _Id(id) {}
    MasterCellType(MasterCellType& a): _MCName(a._MCName) {}

    //Modifier
    void AddPin(const std::string& PinName, int layer)              { _Pins.push_back(PinType(PinName, layer, *this)); }
    void AddBlkg(const std::string& BlkgName, int layer, int demand){ _Blkgs.push_back(BlockageType(BlkgName, layer, demand)); }
    void AddExtraSame(unsigned MC, int demand)                      { _ExtraSameGrid.push_back(MC); _SameGridDemand.push_back(demand); }
    void AddExtraadjH(unsigned MC, int demand)                      { _ExtraadjHGrid.push_back(MC); _adjHGridDemand.push_back(demand); }

    //acceser
    std::string getMCName() const                                   { return _MCName; }
    size_t getNumPins() const                                       { return _Pins.size(); }
    size_t getNumBlkgs() const                                      { return _Blkgs.size(); }
    PinType& getPin(size_t i) const                                 { return _Pins[i]; }
    BlockageType& getBlkg(size_t i) const                           { return _Blkgs[i]; }
    int getDemandSame(unsigned a) const                             
    {
        for(unsigned i = 0, n = _ExtraSameGrid.size(); i < n; ++i)
            if(_ExtraSameGrid[i] == a) return _SameGridDemand[i];
        return 0;
    }
    int getDemandSame(const MasterCellType& a) const
    {
        for(unsigned i = 0, n = _ExtraSameGrid.size(); i < n; ++i)
            if(_ExtraSameGrid[i] == a._Id) return _SameGridDemand[i];
        return 0;
    }
    int getDemandadjH(unsigned a) const                             
    {
        for(unsigned i = 0, n = _ExtraadjHGrid.size(); i < n; ++i)
            if(_ExtraadjHGrid[i] == a) return _adjHGridDemand[i];
        return 0;
    }
    int getDemandadjH(const MasterCellType& a) const
    {
        for(unsigned i = 0, n = _ExtraadjHGrid.size(); i < n; ++i)
            if(_ExtraadjHGrid[i] == a._Id) return _adjHGridDemand[i];
        return 0;
    }

    //friend
    friend std::ostream& operator<<(std::ostream& os, const MasterCellType& MCT);

private:
    const std::string                   _MCName;
    const unsigned                      _Id;
    std::vector<PinType>                _Pins;
    std::vector<BlockageType>           _Blkgs;
    std::vector<unsigned>               _ExtraSameGrid;
    std::vector<int>                    _SameGridDemand;
    std::vector<unsigned>               _ExtraadjHGrid;
    std::vector<int>                    _adjHGridDemand;
};

std::ostream& operator<<(std::ostream& os, const MasterCellType& MCT)
{
    os << "Master Cell Name : " << MCT._MCName << '\n'
       << "Pins : \n";
    for(size_t i = 0, n = MCT._Pins.size(); i < n; ++i) os << MCT._Pins[i];
    os << "Blockages : \n";
    for(size_t i = 0, n = MCT._Blkgs.size(); i < n; ++i) os << MCT._Blkgs[i];
    os << "Same Grid Extra Demand : \n";
    for(size_t i = 0, n = _ExtraSameGrid.size(); i < n; ++i) os << _ExtraSameGrid[i] << " : " << _SameGridDemand[i] << '\n';
    os << "Same Grid Extra Demand : \n";
    for(size_t i = 0, n = _ExtraadjHGrid.size(); i < n; ++i) os << _ExtraadjHGrid[i] << " : " << _adjHGridDemand[i] << '\n';
    os << "---------------------------------------------" << '\n';
    return os;
}

#endif