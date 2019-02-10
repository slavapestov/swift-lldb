// main.swift
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2016 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://swift.org/LICENSE.txt for license information
// See https://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
// -----------------------------------------------------------------------------
class C {}
class D : C {}
protocol P {}

func concrete() {
  var s = String.self //% self.expect("frame var s", DATA_TYPES_DISPLAYED_CORRECTLY, substrs = ["String"])
  var c = D.self //% self.expect("frame var c", DATA_TYPES_DISPLAYED_CORRECTLY, substrs = ["a.D"])
  var t = type(of: (1,2,"hello")) //% self.expect("frame var t", DATA_TYPES_DISPLAYED_CORRECTLY, substrs = ["(Int, Int, String)"])
  var p = P.self //% self.expect("frame var p", DATA_TYPES_DISPLAYED_CORRECTLY, substrs = ["P"])
}

func generic<T, U>(_ t: T, _ u: U.Type) {
  var f = T.self //% self.expect("frame var f", DATA_TYPES_DISPLAYED_CORRECTLY, substrs = ["(Int) -> Int"])
  var g = u //% self.expect("frame var g", DATA_TYPES_DISPLAYED_CORRECTLY, substrs = ["[String]"])
}

concrete()
generic({ (x:Int) -> Int in return x }, [String].self)
