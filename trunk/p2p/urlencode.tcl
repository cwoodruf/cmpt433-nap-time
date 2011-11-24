#!/mnt/remote/tcl/bin/tclsh8.4
proc ue_init {} {
   lappend d + { }
   for {set i 0} {$i < 256} {incr i} {
      set c [format %c $i]
      set x %[format %02x $i]
      if {![string match {[a-zA-Z0-9]} $c]} {
         lappend e $c $x
         lappend d $x $c
      }
   }
   set ::ue_map $e
   set ::ud_map $d
}
ue_init
proc ue {s} { string map $::ue_map $s }
proc ud {s} { string map $::ud_map $s }

puts [ue [read stdin]]

