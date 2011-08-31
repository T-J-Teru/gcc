------------------------------------------------------------------------------
--                                                                          --
--                         GNAT RUN-TIME COMPONENTS                         --
--                                                                          --
--                SYSTEM.MULTIPROCESSORS.DISPATCHING_DOMAINS                --
--                                                                          --
--                                  B o d y                                 --
--                                                                          --
--            Copyright (C) 2011, Free Software Foundation, Inc.            --
--                                                                          --
-- GNARL is free software; you can  redistribute it  and/or modify it under --
-- terms of the  GNU General Public License as published  by the Free Soft- --
-- ware  Foundation;  either version 3,  or (at your option) any later ver- --
-- sion.  GNAT is distributed in the hope that it will be useful, but WITH- --
-- OUT ANY WARRANTY;  without even the  implied warranty of MERCHANTABILITY --
-- or FITNESS FOR A PARTICULAR PURPOSE.                                     --
--                                                                          --
-- As a special exception under Section 7 of GPL version 3, you are granted --
-- additional permissions described in the GCC Runtime Library Exception,   --
-- version 3.1, as published by the Free Software Foundation.               --
--                                                                          --
-- You should have received a copy of the GNU General Public License and    --
-- a copy of the GCC Runtime Library Exception along with this program;     --
-- see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see    --
-- <http://www.gnu.org/licenses/>.                                          --
--                                                                          --
-- GNARL was developed by the GNARL team at Florida State University.       --
-- Extensive contributions were provided by Ada Core Technologies, Inc.     --
--                                                                          --
------------------------------------------------------------------------------

--  Body used on targets where the operating system supports setting task
--  affinities.

with System.Tasking.Initialization;
with System.Task_Primitives.Operations; use System.Task_Primitives.Operations;

with Ada.Unchecked_Conversion;

package body System.Multiprocessors.Dispatching_Domains is

   package ST renames System.Tasking;

   ----------------
   -- Local data --
   ----------------

   Dispatching_Domain_Tasks : array (CPU'First .. Number_Of_CPUs) of Natural :=
                                (others => 0);
   --  We need to store whether there are tasks allocated to concrete
   --  processors in the default system dispatching domain because we need to
   --  check it before creating a new dispatching domain.
   --  ??? Tasks allocated with pragma CPU are not taken into account here.

   Dispatching_Domains_Frozen : Boolean := False;
   --  True when the main procedure has been called. Hence, no new dispatching
   --  domains can be created when this flag is True.

   -----------------------
   -- Local subprograms --
   -----------------------

   function Convert_Ids is new
     Ada.Unchecked_Conversion (Ada.Task_Identification.Task_Id, ST.Task_Id);

   procedure Unchecked_Set_Affinity
     (Domain : ST.Dispatching_Domain_Access;
      CPU    : CPU_Range;
      T      : ST.Task_Id);
   --  Internal procedure to move a task to a target domain and CPU. No checks
   --  are performed about the validity of the domain and the CPU because they
   --  are done by the callers of this procedure (either Assign_Task or
   --  Set_CPU).

   procedure Freeze_Dispatching_Domains;
   pragma Export
     (Ada, Freeze_Dispatching_Domains, "__gnat_freeze_dispatching_domains");
   --  Signal the time when no new dispatching domains can be created. It
   --  should be called before the environment task calls the main procedure
   --  (and after the elaboration code), so the binder-generated file needs to
   --  import and call this procedure.

   -----------------
   -- Assign_Task --
   -----------------

   procedure Assign_Task
     (Domain : in out Dispatching_Domain;
      CPU    : CPU_Range := Not_A_Specific_CPU;
      T      : Ada.Task_Identification.Task_Id :=
                 Ada.Task_Identification.Current_Task)
   is
      Target : constant ST.Task_Id := Convert_Ids (T);

      use type System.Tasking.Dispatching_Domain_Access;

   begin
      --  The exception Dispatching_Domain_Error is propagated if T is already
      --  assigned to a Dispatching_Domain other than
      --  System_Dispatching_Domain, or if CPU is not one of the processors of
      --  Domain (and is not Not_A_Specific_CPU).

      if Target.Common.Domain /= null and then
        Dispatching_Domain (Target.Common.Domain) /= System_Dispatching_Domain
      then
         raise Dispatching_Domain_Error with
           "task already in user-defined dispatching domain";

      elsif CPU /= Not_A_Specific_CPU and then CPU not in Domain'Range then
         raise Dispatching_Domain_Error with
           "processor does not belong to dispatching domain";
      end if;

      --  Assigning a task to System_Dispatching_Domain that is already
      --  assigned to that domain has no effect.

      if Domain = System_Dispatching_Domain then
         return;

      else
         --  Set the task affinity once we know it is possible

         Unchecked_Set_Affinity
           (ST.Dispatching_Domain_Access (Domain), CPU, Target);
      end if;
   end Assign_Task;

   ------------
   -- Create --
   ------------

   function Create (First, Last : CPU) return Dispatching_Domain is
      use type System.Tasking.Dispatching_Domain;
      use type System.Tasking.Dispatching_Domain_Access;
      use type System.Tasking.Task_Id;

      Valid_System_Domain : constant Boolean :=
        (First > CPU'First
          and then
            not (System_Dispatching_Domain (CPU'First .. First - 1) =
                                         (CPU'First .. First - 1 => False)))
                  or else (Last < Number_Of_CPUs
                            and then not
                              (System_Dispatching_Domain
                                (Last + 1 .. Number_Of_CPUs) =
                                  (Last + 1 .. Number_Of_CPUs => False)));
      --  Constant that indicates whether there would exist a non-empty system
      --  dispatching domain after the creation of this dispatching domain.

      T : ST.Task_Id;

      New_Domain : Dispatching_Domain;

   begin
      --  The range of processors for creating a dispatching domain must
      --  comply with the following restrictions:
      --    - Non-empty range
      --    - Not exceeding the range of available processors
      --    - Range from the System_Dispatching_Domain
      --    - Range does not contain a processor with a task assigned to it
      --    - The allocation cannot leave System_Dispatching_Domain empty
      --    - The calling task must be the environment task
      --    - The call to Create must take place before the call to the main
      --      subprogram

      if First > Last then
         raise Dispatching_Domain_Error with "empty dispatching domain";

      elsif Last > Number_Of_CPUs then
         raise Dispatching_Domain_Error with
           "CPU range not supported by the target";

      elsif
        System_Dispatching_Domain (First .. Last) /= (First .. Last => True)
      then
         raise Dispatching_Domain_Error with
           "CPU range not currently in System_Dispatching_Domain";

      elsif
        Dispatching_Domain_Tasks (First .. Last) /= (First .. Last => 0)
      then
         raise Dispatching_Domain_Error with "CPU range has tasks assigned";

      elsif not Valid_System_Domain then
         raise Dispatching_Domain_Error with
           "would leave System_Dispatching_Domain empty";

      elsif Self /= Environment_Task then
         raise Dispatching_Domain_Error with
           "only the environment task can create dispatching domains";

      elsif Dispatching_Domains_Frozen then
         raise Dispatching_Domain_Error with
           "cannot create dispatching domain after call to main program";
      end if;

      New_Domain := new ST.Dispatching_Domain'(First .. Last => True);

      --  At this point we need to fix the processors belonging to the system
      --  domain, and change the affinity of every task that has been created
      --  and assigned to the system domain.

      ST.Initialization.Defer_Abort (Self);

      Lock_RTS;

      System_Dispatching_Domain (First .. Last) := (First .. Last => False);

      --  Iterate the list of tasks belonging to the default system
      --  dispatching domain and set the appropriate affinity.

      T := ST.All_Tasks_List;

      while T /= null loop
         if T.Common.Domain = null or else
           T.Common.Domain = ST.System_Domain
         then
            Set_Task_Affinity (T);
         end if;

         T := T.Common.All_Tasks_Link;
      end loop;

      Unlock_RTS;

      ST.Initialization.Undefer_Abort (Self);

      return New_Domain;
   end Create;

   -----------------------------
   -- Delay_Until_And_Set_CPU --
   -----------------------------

   procedure Delay_Until_And_Set_CPU
     (Delay_Until_Time : Ada.Real_Time.Time;
      CPU              : CPU_Range)
   is
   begin
      --  Not supported atomically by the underlying operating systems.
      --  Operating systems use to migrate the task immediately after the call
      --  to set the affinity.

      delay until Delay_Until_Time;
      Set_CPU (CPU);
   end Delay_Until_And_Set_CPU;

   --------------------------------
   -- Freeze_Dispatching_Domains --
   --------------------------------

   procedure Freeze_Dispatching_Domains is
   begin
      --  Signal the end of the elaboration code

      Dispatching_Domains_Frozen := True;
   end Freeze_Dispatching_Domains;

   -------------
   -- Get_CPU --
   -------------

   function Get_CPU
     (T : Ada.Task_Identification.Task_Id :=
            Ada.Task_Identification.Current_Task) return CPU_Range
   is
   begin
      return Convert_Ids (T).Common.Base_CPU;
   end Get_CPU;

   ----------------------------
   -- Get_Dispatching_Domain --
   ----------------------------

   function Get_Dispatching_Domain
     (T : Ada.Task_Identification.Task_Id :=
            Ada.Task_Identification.Current_Task) return Dispatching_Domain
   is
   begin
      return Dispatching_Domain (Convert_Ids (T).Common.Domain);
   end Get_Dispatching_Domain;

   -------------------
   -- Get_First_CPU --
   -------------------

   function Get_First_CPU (Domain : Dispatching_Domain) return CPU is
   begin
      for Proc in Domain'Range loop
         if Domain (Proc) then
            return Proc;
         end if;
      end loop;

      --  Should never reach the following return

      return Domain'First;
   end Get_First_CPU;

   ------------------
   -- Get_Last_CPU --
   ------------------

   function Get_Last_CPU (Domain : Dispatching_Domain) return CPU is
   begin
      for Proc in reverse Domain'Range loop
         if Domain (Proc) then
            return Proc;
         end if;
      end loop;

      --  Should never reach the following return

      return Domain'Last;
   end Get_Last_CPU;

   -------------
   -- Set_CPU --
   -------------

   procedure Set_CPU
     (CPU : CPU_Range;
      T   : Ada.Task_Identification.Task_Id :=
              Ada.Task_Identification.Current_Task)
   is
      Target : constant ST.Task_Id := Convert_Ids (T);

      use type ST.Dispatching_Domain_Access;

   begin
      --  The exception Dispatching_Domain_Error is propagated if CPU is not
      --  one of the processors of the Dispatching_Domain on which T is
      --  assigned (and is not Not_A_Specific_CPU).

      if CPU /= Not_A_Specific_CPU and then
        (CPU not in Target.Common.Domain'Range or else
         not Target.Common.Domain (CPU))
      then
         raise Dispatching_Domain_Error with
           "CPU does not belong to the task's dispatching domain";
      end if;

      Unchecked_Set_Affinity (Target.Common.Domain, CPU, Target);
   end Set_CPU;

   ----------------------------
   -- Unchecked_Set_Affinity --
   ----------------------------

   procedure Unchecked_Set_Affinity
     (Domain : ST.Dispatching_Domain_Access;
      CPU    : CPU_Range;
      T      : ST.Task_Id)
   is
      Source_CPU : constant CPU_Range := T.Common.Base_CPU;

      use type System.Tasking.Dispatching_Domain_Access;

   begin
      Write_Lock (T);

      --  Move to the new domain

      T.Common.Domain := Domain;

      --  Attach the CPU to the task

      T.Common.Base_CPU := CPU;

      --  Change the number of tasks attached to a given task in the system
      --  domain if needed.

      if not Dispatching_Domains_Frozen
        and then (Domain = null or else Domain = ST.System_Domain)
      then
         --  Reduce the number of tasks attached to the CPU from which this
         --  task is being moved, if needed.

         if Source_CPU /= Not_A_Specific_CPU then
            Dispatching_Domain_Tasks (Source_CPU) :=
              Dispatching_Domain_Tasks (Source_CPU) - 1;
         end if;

         --  Increase the number of tasks attached to the CPU to which this
         --  task is being moved, if needed.

         if CPU /= Not_A_Specific_CPU then
            Dispatching_Domain_Tasks (CPU) :=
              Dispatching_Domain_Tasks (CPU) + 1;
         end if;
      end if;

      --  Change the actual affinity calling the operating system level

      Set_Task_Affinity (T);

      Unlock (T);
   end Unchecked_Set_Affinity;

end System.Multiprocessors.Dispatching_Domains;
