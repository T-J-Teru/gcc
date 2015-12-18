; the decode patterns can be expressed either as having an additive constant
; in the shift count, or as shifting a power of two instead of just 1.
; The former matches a combination of a (presumably previously combined)
; bset pattern; the latter is simpler RTL, and also needed when the additive
; constant would be zero (i.e. the power of two is the zeroth, i.e. 1).
(define_insn "*decode_p0_plus"
  [(set (match_operand:SI 0 "register_operand" "=Rrq")
	(ior:SI (and:SI (match_operand:SI 1 "register_operand" "0")
			(match_operand:SI 2 "const_int_operand" "n"))
		(ashift:SI (const_int 1)
		  (plus:SI (match_operand:SI 3 "register_operand" "Rrq")
			   (match_operand:SI 4 "const_int_operand" "n")))))]
  "TARGET_DECODE && arc_decode_p_size (operands[2], operands[4]) >= 0"
{
  rtx xop[4];

  xop[0] = operands[0];
  xop[1] = operands[3];
  xop[2] = operands[4];
  xop[3] = GEN_INT (arc_decode_p_size (operands[2], operands[4]));
  output_asm_insn ("decode %0,%0,%1,%2,0,%3", xop);
  return "";
}
  [(set_attr "type" "shift")
   (set_attr "length" "4")])

(define_insn "*decode_p0"
  [(set (match_operand:SI 0 "register_operand" "=Rrq")
	(ior:SI (and:SI (match_operand:SI 1 "register_operand" "0")
			(match_operand:SI 2 "const_int_operand" "n"))
		(ashift:SI (match_operand:SI 4 "const_int_operand" "n")
			   (match_operand:SI 3 "register_operand" "Rrq"))))]
  "TARGET_DECODE && arc_decode_size (operands[2], operands[4]) >= 0"
{
  rtx xop[4];

  xop[0] = operands[0];
  xop[1] = operands[3];
  xop[2] = operands[4];
  xop[3] = GEN_INT (arc_decode_size (operands[2], operands[4]));
  output_asm_insn ("decode %0,%0,%1,%z2,0,%3", xop);
  return "";
}
  [(set_attr "type" "shift")
   (set_attr "length" "4")])

(define_insn "*decode_pn_plus"
  [(set (match_operand:SI 0 "register_operand" "=Rrq")
	(ior:SI (and:SI (match_operand:SI 1 "register_operand" "0")
			(match_operand:SI 2 "const_int_operand" "n"))
		(ashift:SI (const_int 1)
		  (plus:SI (zero_extract:SI
			     (match_operand:SI 3 "register_operand" "Rrq")
			     (const_int 5)
			     (match_operand:SI 5 "const_int_operand" "n"))
			   (match_operand:SI 4 "const_int_operand" "n")))))]
  "TARGET_DECODE && arc_decode_p_size (operands[2], operands[4]) >= 0"
{
  rtx xop[5];

  xop[0] = operands[0];
  xop[1] = operands[3];
  xop[2] = operands[4];
  xop[3] = GEN_INT (arc_decode_p_size (operands[2], operands[4]));
  xop[4] = operands[5];
  output_asm_insn ("decode %0,%0,%1,%2,%4,%3", xop);
  return "";
}
  [(set_attr "type" "shift")
   (set_attr "length" "4")])

(define_insn "*decode_pn"
  [(set (match_operand:SI 0 "register_operand" "=Rrq")
	(ior:SI (and:SI (match_operand:SI 1 "register_operand" "0")
			(match_operand:SI 2 "const_int_operand" "n"))
		(ashift:SI (match_operand:SI 4 "const_int_operand" "n")
			   (zero_extract:SI
			     (match_operand:SI 3 "register_operand" "Rrq")
			     (const_int 5)
			     (match_operand:SI 5 "const_int_operand" "n")))))]
  "TARGET_DECODE && arc_decode_size (operands[2], operands[4]) >= 0"
{
  rtx xop[5];

  xop[0] = operands[0];
  xop[1] = operands[3];
  xop[2] = operands[4];
  xop[3] = GEN_INT (arc_decode_size (operands[2], operands[4]));
  xop[4] = operands[5];
  output_asm_insn ("decode %0,%0,%1,%z2,%4,%3", xop);
  return "";
}
  [(set_attr "type" "shift")
   (set_attr "length" "4")])

;; If there is no zero extract, we can just shift a constant (a power of two)
;; by the register input; the constant could be subject to cse / loop
;; hoisting, and asl gives the register allocator more freedom, so the use
;; of decode.cl in that case would be questionable.
(define_insn "*decode_cl"
  [(set (match_operand:SI 0 "register_operand" "=Rrq")
	(ashift:SI (match_operand:SI 2 "const_int_operand" "n")
		   (zero_extract:SI
		     (match_operand:SI 1 "register_operand" "Rrq")
		     (const_int 5)
		     (match_operand:SI 3 "const_int_operand" "n"))))]
  "TARGET_DECODE && IS_POWEROF2_P (INTVAL (operands[2]))"
  "decode.cl %0,%1,%z2,%3"
  [(set_attr "type" "shift")
   (set_attr "length" "4")])

; In this case, canonicalization inside combine doesn't go far enough, so
; give this alternate pattern.
(define_insn "*decode_cl_plus"
  [(set (match_operand:SI 0 "register_operand" "=Rrq")
	(ashift:SI (const_int 1)
	  (plus:SI (zero_extract:SI
		     (match_operand:SI 1 "register_operand" "Rrq")
		     (const_int 5)
		     (match_operand:SI 3 "const_int_operand" "n"))
		   (match_operand:SI 2 "const_int_operand" "n"))))]
  "TARGET_DECODE"
  "decode.cl %0,%1,%2,%3"
  [(set_attr "type" "shift")
   (set_attr "length" "4")])
