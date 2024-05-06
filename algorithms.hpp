///////////////////////////////////////////////////////////////////////////////
// algorithms.hpp
//
// Definitions for four algorithms:
//
// run_length_encode
// longest_frequent_substring
// reformat_date
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <map>
#include <vector>
#include <math.h>

namespace algorithms {
  void append_run(std::string&, char, int);
  bool valid_input_one_two(std::string, int, int);
  // Run-length-encode the given string.
  //
  // uncompressed must be a string containing only lower-case letters or spaces.
  //
  // A run is defined as a sequence of K>2 contiguous copies of the same
  // character c.
  // For example, "aaa" is a run with K=3 and c='a'.
  // This function returns a string based on uncompressed, where every run is
  // replaced with the string
  //   COUNTc
  // where COUNT is the base-10 representation of K. Non-run characters are
  // left as-is.
  //
  // Example inputs and outputs:
  //   "aaa" -> "3a"
  //   "heloooooooo there" -> "hel8o there"
  //   "footloose and fancy free" -> "f2otl2ose and fancy fr2e"
  //
  // Throws std::invalid_argument if the string contains invalid characters.
  std::string run_length_encode(const std::string& uncompressed) {

    std::string encoded_string = "";
    //Base Case if input is empty return empty string
    if(uncompressed.length() == 0){
      return encoded_string;
    }
    //Base Case if input is 1 character return it
    if(uncompressed.length() == 1){
      return uncompressed;
    }
    //iterates through string to check if string contains invalid characters, returns invalid if found.
    for(int i = 0; i < uncompressed.length(); i++){
      if(uncompressed[i] < 97 || uncompressed[i] > 122){
        if(uncompressed[i] != ' '){
          throw std::invalid_argument("Invalid");
        }
      }
    }
    char run_char = uncompressed[0];
    int run_length = 1;
    for(int i = 1; i < uncompressed.length(); i++){
      //checks how many times a character is repeated
      if(uncompressed[i] == run_char){
        run_length++;
      }
      else{
        //helper function to add combined characters if there is more than 1
        append_run(encoded_string, run_char, run_length);
        run_char = uncompressed[i];
        run_length = 1;
      }
    }
    append_run(encoded_string, run_char, run_length);
    return encoded_string;
  }
  //Helper function converts int to string and adds it to new string
  void append_run(std::string& encoded_string, char run_char, int run_length){
    if(run_length > 1){
      encoded_string += std::to_string(run_length);
    }
    encoded_string += run_char;
  }

  // Returns the longest substring of text, such that every character in the
  // substring appears at least k times in text.
  // If there are ties, the substring that appears first is returned.
  std::string longest_frequent_substring(const std::string& text, unsigned k) {

    //Base cases if the length of text is lower than the frequency, return empty string
    //If text string is empty return empty string
    if(text.length() < k || text.length() == 0){
      return "";
    }
    //Bases cases if the frequency is one or 0, return the string
    if(k == 1 || k == 0){
      return text;
    }
    //Map that holds each character and its frequency
    std::map<char,int> freq;
    //Loop to add the characters into the map and its frequency
    for(int i = 0; i < text.length(); i++){
      //If a character does not exist in the map than add it to the map
      if(!freq[text[i]]){
        freq[text[i]] = 1;
      }
      else{
        freq[text[i]]++;
      }
    }
    std::string best = "";
    std::string cand = "";
    //loop through the string
    for(int b = 0; b < text.length() - 1; b++){
      //boolean to hold whether every character in cand is greater than k
      bool cand_is_greater = false;
      //Loop through the string to get subset
      for(int e = b; e < text.length(); e++){
        //string that is set to hold the subset of b the side of e-b+1
        cand = text.substr(b,e-b+1);
        //if statement to check if the frequency of a character in cand is less than k
        if((freq[cand[e - b]] < k)){
          //terminates the loop if frequency is lower
          e = text.length() - 1;
          //boolean is set to false to avoid changing best
          cand_is_greater = false;
        }
        else{
          cand_is_greater = true;
        }
        //if cand is larger than best and the frequency of every character in c is larger than k best is changed.
        if(cand_is_greater == true && cand.length() > best.length()){
          best = cand;
        }
      }
    }
    return best;
  }

  // Reformats a string containing a date into YYYY-MM-DD format.
  //
  // input may be formatted in one of four patterns:
  // 1. Y-M-D
  //    where Y, M, and D are positive integers
  // 2. M/D/Y
  //    where Y, M, and D are positive integers
  // 3. MONTH DAY, YEAR
  //    where
  //    - MONTH is a case-insensitive name of a month, ex.
  //      "january", "FEBRUARY", "March", etc.
  //    - DAY is a positive integer representing a day, e.x. "15"
  //    - YEAR is a positive integer representing a year, e.x. "2022"
  // 4. MON DAY, YEAR
  //    where
  //    - MON is a case-insensitive three-letter abbreviation of a month, ex.
  //      "jan", "FEB", "Mar", etc.
  //    - DAY is the same as above
  //    - YEAR is the same as above
  //
  // Any leading spaces or trailing spaces are ignored.
  //
  // Returns a string in strict YYYY-MM-DD format.
  //
  // Throws std::invalid argument if:
  // - input does not fit any of the four patterns
  // - MONTH is not a valid month name
  // - MON is not a valid month abbreviation
  // - DAY is not in the range [1, 31]
  // - YEAR is not in the range [1900, 2099]
  std::string reformat_date(const std::string& input) {

    //strings and ints to hold base values
    std::string months_abbreviation[12] = {"jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec"};
    std::string months_full[12] = {"january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december"};
    int years[2] = {1900, 2099};
    int months[2] = {1,12};
    int days[2] = {1,31};
    std::string output = "";
    std::vector<std::string> format;
    std::string temp = "";
    char decision = input[0];
    //loop through the input to store values into a vector and remove commas, spaces, slashes, and dashes
    for(int i = 0; i < input.length(); i++){
      if(input[i] == '/' || input[i] == '-' || input[i] == ' ' || input[i] == ','){
        if(input[i] == '/' || input[i] == '-' || input[i] == ','){
          decision = input[i];
        }
        //adds values into vector when string is not empty
        if(temp != ""){
          format.push_back(temp);
        }
          temp = "";
        }
        else{
          //characters are lowered to avoid case sensitivity
          temp += tolower(input[i]);
        }
        //pushes last value into vector
        if(i == input.length() - 1){
          if(temp != ""){
            format.push_back(temp);
          }
        }
    }
    //if vector does not have 3 values representing year day month, throw invalid
    if(format.size() != 3){
      throw std::invalid_argument("invalid");
    }
    //if input string contained - or / use the first two pattern options
    if(decision == '-' || decision == '/'){
      //loop through vector
      for(int i = 0; i < 3; i++){
        //if input contained '-', send to helper function and check if it is valid depending on location of element
        if((i == 0 && decision == '-')|| (i == 2 && decision == '/')){
          //if valid, place into output string
          if(valid_input_one_two(format[i], years[0], years[1])){
              if(decision == '-'){
                  output += format[i] + '-';
              }
              if(decision == '/'){
                  output = format[i] + '-' + output;
              }
          }
        }
        if((i == 1 && decision == '-') || (i == 0 && decision == '/')){
          if(valid_input_one_two(format[i], months[0], months[1])){
            //if element is length of 1 add 0 to format output, otherwise if length is 2 add to output without formatting and input had '-'
            if(decision == '-' && format[i].length() == 1){
              output += '0' + format[i] + '-';
            }
            else if(decision == '-'){
              output += format[i] + '-';
            }
            //if element is length of 1 add 0 to format output, otherwise if length is 2 add to output without formatting and input had '/'
            else if(decision == '/' && format[i].length() == 1){
                output += '0' + format[i] + '-';
            }
            else if(decision == '/'){
                output += format[i] + '-';
            }
          }
        }
        if((i == 2 && decision == '-') || (i == 1 && decision == '/')){
          if(valid_input_one_two(format[i], days[0], days[1])){
            //if element is length of 1 add 0 to format output, otherwise if length is 2 add to output without formatting and input had '-'
            if(decision == '-' && format[i].length() == 1){
                output += '0' + format[i];
            }
            else if(decision == '-'){
                output += format[i];
            }
            //if element is length of 1 add 0 to format output, otherwise if length is 2 add to output without formatting and input had '/'
            else if(decision == '/' && format[i].length() == 1){
              output += '0' + format[i];
            }
            else if (decision == '/'){
              output += format[i];
            }
          }
        }
      }
    }
    //boolean to check if month exists
    bool month_exist = false;
    //if input contained ',', send to helper function and check if it is valid depending on location of element
    if(decision == ','){
      for(int i = 0; i < 3; i++){
        if(i == 0){
          if(month_exist == false){
            //loops through months 
            for(int n = 0; n < 12; n++){
              if(format[i] == months_full[n] || format[i] == months_abbreviation[n]){
                //if the month is found, boolean goes to true to skip next loop
                month_exist = true;
                //if month is below or equal to 9, add 0 infront for formatting, otherwise ignore 0
                if(n <= 8){
                  output = "-0" + std::to_string(n+1) + '-';
                }
                else{
                  output = '-' + std::to_string(n+1) + '-';
                }
              }
            }
          }
        }
        //if valid month name was not found, throw invalid argument
        if(month_exist == false){
          throw std::invalid_argument("Invalid");
        }
        if(i == 1){
          if(valid_input_one_two(format[i], days[0], days[1])){
            //if element is length of 1 add 0 to format output, otherwise if length is 2 add to output without formatting
            if(format[i].length() == 1){
              output = output + '0' + format[i];
            }
            else{
              output = output + format[i];
            }
          }
        }
        if(i == 2){
          if(valid_input_one_two(format[i], years[0], years[1])){
            output = format[i] + output;
          }
        }
      }
    }
    return output;

  }
  //Helper function to validate years months and days
  bool valid_input_one_two(std::string chunk_of_date, int valinterval_low, int valinterval_max){
    //helps change year from string to int to compare if valid
    int begin_to_end = chunk_of_date.length() - 1;
    int temp_time = 0;
    //loop through string to convert to int
    for(int i = 0; i < chunk_of_date.length(); i++){
        temp_time += (chunk_of_date[i] - 48) * pow(10, begin_to_end - i);
    }
    //checks if int is valid, if not invalid arguent is thrown
    if(temp_time >= valinterval_low && temp_time <= valinterval_max){
        return true;
    }
    else{
        throw std::invalid_argument("Invalidd");
    }
    return false;

  }
}
