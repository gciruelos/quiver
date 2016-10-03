" Vim syntax file
" Language: Quiver
" Maintainer: Gonzalo Ciruelos
" Latest Revision: 03 October 2016

if exists("b:current_syntax")
  finish
endif

syn keyword quiverKeywords p contained
syn match quiverNumber '\d*'
syn region quiverCondition start='?' end='?'
syn region quiverString start='\'' end=')' contained extend
syn region quiverEffect start='(' end=')' contains=quiverNumber,quiverString extend

let b:current_syntax = "quiver"

hi def link quiverCondition   Conditional
hi def link quiverNumber      Number
hi def link quiverString      String
hi def link quiverEffect      Function
hi def link quiverKeywords    Keyword

