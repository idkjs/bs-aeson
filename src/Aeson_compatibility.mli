module Either : sig
  type ('a, 'b) t = Left of 'a | Right of 'b
  (** the type parameter order of Belt.Result.t is backwards from Either *)
  
  val left : 'a -> ('a, 'b) t
  val right : 'b -> ('a, 'b) t

  val either : ('a -> 'c) -> ('b -> 'c) -> ('a, 'b) t -> 'c

  val to_result : ('a, 'b) t -> ('b, 'a) Belt.Result.t
  val of_result : ('b, 'a) Belt.Result.t -> ('a, 'b) t
  
  val map : ('b -> 'c) -> ('a, 'b) t -> ('a, 'c) t
  val (<$>) : ('b -> 'c) -> ('a, 'b) t -> ('a, 'c) t
  val map_left : ('a -> 'c) -> ('a, 'b) t -> ('c, 'b) t

  val bimap : ('a -> 'c) -> ('b -> 'd) -> ('a, 'b) t -> ('c, 'd) t

  val pure : 'b -> ('a, 'b) t
  val apply : ('a, ('b -> 'c)) t -> ('a, 'b) t -> ('a, 'c) t
  val (<*>) : ('a, ('b -> 'c)) t -> ('a, 'b) t -> ('a, 'c) t

  val return : 'b -> ('a, 'b) t
  val bind : ('a, 'b) t -> ('b -> ('a, 'c) t) -> ('a, 'c) t

  val (>>=) : ('a, 'b) t -> ('b -> ('a, 'c) t) -> ('a, 'c) t
  val throw : 'a -> ('a, 'b) t

  val is_left : ('a, 'b) t -> bool
  val is_right : ('a, 'b) t -> bool

  val to_string : ('a -> string) -> ('b -> string) -> ('a, 'b) t -> string

  val error : (exn, 'a) t -> 'a

  val try_ : (unit -> 'b) -> (exn, 'b) t

  val hush : ('a, 'b) t -> 'b option
  val note : 'a -> 'b option -> ('a, 'b) t

  val fold : ('b -> 'c -> 'c) -> 'c -> ('a, 'b) t -> 'c
end