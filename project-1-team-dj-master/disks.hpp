///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// 335-04
// Project 1
//
// Jessica Vargas
// Dylan N
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>


#include <functional>
#include <iostream>

enum disk_color { DISK_DARK, DISK_LIGHT};

class disk_state {
private:
  std::vector<disk_color> _colors;

public:
  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_DARK) {

    assert(light_count > 0);

    for (size_t i = 0; i < _colors.size(); i += 2) {
      _colors[i] = DISK_LIGHT;
    }
  }

  bool operator== (const disk_state& rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const {
    return _colors.size();
  }

  size_t light_count() const {
    return total_count() / 2;
  }

  size_t dark_count() const {
    return light_count();
  }

  bool is_index(size_t i) const {
    return (i < total_count());
  }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

 

  //Returns true when disks are alternating Light-Dark-Light-Dark etc
  bool is_initialized() const {
    
    //create placeholder to switch colors
    disk_color colorCheck = DISK_LIGHT;
    size_t n = _colors.size();
    //loop through the size of _colors, if it doesn't match the correct color it is false
    //colorCheck alternates between DISK_LIGHT and DISK_DARK
    for (size_t i = 0; i < n ; i ++){
        if(_colors[i] != colorCheck)
        {
          return false;
        }
        if(colorCheck == DISK_LIGHT)
        {
          colorCheck = DISK_DARK;
        }
        else{
          colorCheck = DISK_LIGHT;
        }
    }

    return true;
  }

  

  //True when all dark disks are on the left and all light disks are on the right
  //for total_count()/2 the lower indicies would be dark, higher ones would be light
  bool is_sorted() const {
  
  size_t colorsHalf = total_count()/2;

  for(size_t j = 0; j < colorsHalf; j++)
    {
      if(_colors[j]==DISK_LIGHT)
      {
        return false;
      }
    }
    for(size_t i = colorsHalf; i < total_count(); i++)
    {
      if(_colors[i]==DISK_DARK)
      {
        return false;
      }
    }

    return true;  
  }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;
  
public:
  
  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }
  
  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }
  
  const disk_state& after() const {
    return _after;
  }
  
  unsigned swap_count() const {
    return _swap_count;
  }
};

// Alternate Algorithm. Sorts Left->Right only, with odd runs go end to end 
//even runs go second in to second last
//if current disk is LIGHT and next disk is DARK, then swap
sorted_disks sort_alternate(const disk_state& before) {
  
int count=0;  //number of swaps
int pos=0;    //position in the disks
disk_state disk=before;
size_t n = disk.total_count();
//create copy of before

//check if disk is populated with alternating disks
assert(disk.is_initialized());

//alternating algo has n number of runs, with n being the total count
for(size_t i=0; i < n; i++)
{
  
//Runs 1 & 3, end to end
  for(size_t f=0; f < disk.dark_count(); f++)
  {
      if(disk.get(pos)==DISK_LIGHT && disk.get(pos+1)==DISK_DARK)
      {
        disk.swap(pos);
        count++;
        
      }
      pos+=2; //increment by pair, moves by 2
  }
  pos=1;  //resetting for Run 2 & 4

//Runs 2 & 4, second in to second last
  for(size_t g=0; g < disk.dark_count()-1; g++)
  {
      if(disk.get(pos)==DISK_LIGHT && disk.get(pos+1)==DISK_DARK)
      {
        disk.swap(pos);
        count++;
      }
      pos+=2;
  }
  pos=0; //resetting for 1 & 3
}

return sorted_disks(disk, count);
}


// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) {
  

    disk_state disk = before;

    assert(disk.is_initialized());

    //Set swap counter
    int counter = 0;

    //Create placeholder for n/2 movement
    int loopAmnt = disk.total_count() / 2;

    //Loop N/2 amount of times
    for (int x = 0; x < loopAmnt; x++) 
    {
        //loop front to back
        for (int i = 0; i < loopAmnt; i++) {
            if (disk.get(i) == DISK_LIGHT && disk.get(i++) == DISK_DARK) 
            {
                disk.swap(i);
                counter++;
            }

        }
        //loop back to front
        for (int j = disk.total_count()-1; j > 0; j--) 
        {
            if (disk.get(j) == DISK_DARK && disk.get(j-1) == DISK_LIGHT)
            {
                disk.swap(j-1);
                counter++;
            }
           
        }
    }
    return sorted_disks(disk, counter);

}


  
