  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! The following pre-processor variables are currently used when
  ! included:
  !  - BUD_TYPE_NAME of the type
  !  - BUD_TYPE_NAME_, internal data pointer of the type
  !  - BUD_TYPE_NAME_STR, the routine name in "string" format (for IO)
  !  - BUD_TYPE_NEW, the routine name for creating a new type
  !  - BUD_FSTACK_TYPE, the variable type contained in the stack

  ! Include the common utilities
#include "bud_utils.inc"

  !> @cond BUD_DOC_INC

  !> @defgroup bud-fstack Finite stack of arbitrary @bud
  !!
  !! The finite stack implementation is an extension of the regular
  !! reference counted objects.
  !!
  !! A stack is a "push-pop" sequence of @buds.
  !! A `push` of a new @bud is inserted at the top, if the stack is
  !! full the oldest entry is deleted and removed from the stack.
  !!
  !! A `pop` of a @bud removes the top entry of the stack and possibly
  !! returns the @bud.
  !!
  !! @note
  !! A type need not have _all_ these routines available if so desired.
  !! This is internally controlled and is mentioned for the relevant types.

  !> @endcond BUD_DOC_INC

  !> Finite stack with cyclic ability
  !!
  !! Container for a finite stack of a single @bud (BUD_FSTACK_TYPE).
  !!
  !! This data container holds 3 variables,
  !!  1. track maximum number of elements allowed in the finite stack
  !!  2. track how many elements are currently stored
  !!  3. An array of the stack of variables stored.
  type BUD_TYPE_NAME

    !> @cond BUD_DEVELOPER

    !> Stored pointer which contains the reference counting etc.
    type(BUD_TYPE_NAME_), pointer :: D => null()

    !> @endcond BUD_DEVELOPER

#   include "bud_common_type.inc"
#if BUD_FORTRAN >= 2003

    !> @name Private procedures
    !> @{
    ! Doxygen needed line
    procedure, private :: pop_, pop_delete_
    !> @}

    !> @iSee #new
    procedure, public :: new => new_
    !> @iSee #push
    procedure, public :: push => push_
    !> @iSee #pop
    generic,   public :: pop => pop_, pop_delete_
    !> @iSee #clear
    procedure, public :: clear => clear_
    !> @iSee #retain
    procedure, public :: retain => retain_

    !> @iSee #element
    procedure, public :: element => get_elem_
    !> @iSee #element
    procedure, public :: get => get_elem_

    !> @iSee #element_p
    procedure, public :: element_p => get_elem_p_
    !> @iSee #element_p
    procedure, public :: get_p => get_elem_p_

    !> @iSee #size
    procedure, public :: size => size_elem_
    !> @iSee #size_max
    procedure, public :: size_max => size_max_elem_

#endif
  end type BUD_TYPE_NAME


  !> @cond BUD_DEVELOPER

  !> @bud container for BUD_TYPE_NAME
  type BUD_TYPE_NAME_

    !> Maximum number of elements that can be stored in this stack.
    !!
    !! @note
    !! There is little to no memory overhead of having an excessively
    !! large `max` as all elements which have not been assigned are
    !! allocations of data-pointers.
    !!
    !! Hence creating extreme stacks may still be memory efficient.
    integer :: nel_max

    !> Current number of elements in the stack.
    integer :: nel

    !> Array of elements (stack).
    type(BUD_FSTACK_TYPE), pointer :: T(:) => null()

    ! Consistent data in the reference counted object
#   include "bud_common_type_.inc"

  end type BUD_TYPE_NAME_

  !> @endcond BUD_DEVELOPER


  ! special counter for deleting all elements
  ! of a stack using:
  !   call pop(<this>, 0)
  !> Special parameter for popping all elements.
  !!
  !! Allows one to pop/delete all elements in the stack.
  !! This basically performs a
  !! \code{.f90}
  !!  n = size_max(this)
  !!  call delete(this)
  !!  call new(this, n)
  !! \endcode
  !!integer, public, parameter :: BUD_FSTACK_ALL = 0


  !> Internal initialization of finite stack holding BUD_FSTACK_TYPE
  !!
  !! If called on a previously allocated data scope this
  !! will clear the reference (@isee delete).
  !! Subsequently the type container will be allocated
  !! and the reference counted data will be 1.
  interface new
    module procedure new_
  end interface
  public :: new

  !> Adds/Pushes new element to the stack
  !!
  !! If the stack is full the oldest element will be deleted
  !! and the second oldest becomes the first element of the stack.
  !!
  !! @note
  !! One cannot push elements in the middle of the stack.
  interface push
    module procedure push_
  end interface
  public :: push

  !> Pop and (possibly) return the latest item in the stack
  !!
  !! The top item is returned in `item` and the stack
  !! is decremented thus removing the returned item from the stack.
  !!
  !! An optional index may be given, see #pop_delete_.
  !!
  !! If the top element has not been initialized the returned item will
  !! be un-initialized.
  !!
  !! @note
  !! The returned item will thus not have its counted references changed.
  interface pop
    module procedure pop_
    module procedure pop_delete_
  end interface
  public :: pop

  !> Retains a fixed number of elements in the stack
  !!
  !! Retains a number of elements from either the top or the
  !! bottom of the stack.
  !!
  !! If any of the counts are larger than the currently
  !! number of stored elements, nothing will happen.
  interface retain
    module procedure retain_
  end interface
  public :: retain

  !> Clears the stack so all contained elements are deleted
  !!
  !! This is similar to `call pop(this, 0)`.
  interface clear
    module procedure clear_
  end interface
  public :: clear

  !> Retrieve specific element from the stack
  !!
  !! If the requested element is beyond the number of
  !! stored elements `elem` will be un-initialized upon return.
  interface element
    module procedure get_elem_
  end interface
  public :: element

  !> Retrieve pointer to specific element in the stack
  !!
  !! Retrieving a pointer does not change the reference status
  !! of the @bud.
  !!
  !! If the requested element is beyond the number of
  !! stored elements `elem` will be null.
  !!
  interface element_p
    module procedure get_elem_p_
  end interface
  public :: element_p

  !> Query number of currently stored elements in this stack
  interface size
    module procedure size_elem_
  end interface
  public :: size

  !> Query maximum number of allowed elements in this stack
  interface size_max
    module procedure size_max_elem_
  end interface
  public :: size_max


  ! all default reference counted stuff
# include "bud_common.inc"

  !> @cond BUD_DEVELOPER

  subroutine delete_(this)
    type(BUD_TYPE_NAME), intent(inout) :: this
    integer :: i, stat

    stat = 0
    if ( associated(this%D%T) ) then

      ! Deallocate array
      do i = 1 , size(this%D%T)
        call delete(this%D%T(i))
        if ( 0 /= error(this%D%T(i)) ) then
          stat = error(this%D%T(i))
        end if
      end do
      deallocate(this%D%T, stat=i)
      if ( 0 /= i ) stat = i
      nullify(this%D%T)

    end if

    this%D%nel_max = 0
    this%D%nel = 0
    call set_error(this, stat)

  end subroutine delete_

  !> @endcond BUD_DEVELOPER


  !> @param[in] from the original `bud` which is copied to `to`
  !! @param[inout] to the output `bud` with the full copied data
  subroutine copy_(from, to)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to
    integer :: i
    type(BUD_FSTACK_TYPE), pointer :: p
    type(BUD_FSTACK_TYPE) :: cp

    call delete(to)
    if ( .not. is_initd(from) ) return

    call new(to, from%D%nel_max)

    do i = 1 , size(from)
      p => element_p(from, i)
      call copy(p, cp)
      call push(to, cp)
      call delete(cp)
    end do

    ! Copy the common data-content
    call common_copy_(from, to)

  end subroutine copy_

  !> @param[inout] this initialize the finite stack
  !! @param[in] nel maximum number of elements that may be contained in the stack
  subroutine new_(this, nel)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer, intent(in) :: nel

    if ( nel < 1 ) then
      call set_error(this, -1)
      return
    end if

    call initialize(this)

    this%D%nel_max = nel
    this%D%nel = 0

    ! prepare data locations
    allocate(this%D%T(nel))

  end subroutine new_


  !> @param[inout] this stack that gets added an element
  !! @param[in] item element added to the stack
  subroutine push_(this, item)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    type(BUD_FSTACK_TYPE), intent(in) :: item

    integer :: el, i

    ! if this has not been initialised we immediately return
    ! We _could_ initialize it, but we do not know how
    ! many maximum elements that is allowed?
    ! So we return
    if ( .not. is_initd(this) ) return

    el = size(this)

    if ( el == size_max(this) ) then

      ! cycle stack and push
      do i = 2 , el
        this%D%T(i-1) = this%D%T(i)
      end do
      ! Assignment will also delete.
      this%D%T(el) = item

    else

      el = el + 1
      ! simply add it to the stack
      this%D%nel = el
      this%D%T(el) = item

    end if

  end subroutine push_


  !> @param[inout] this stack that gets removed an item
  !! @param[inout] item returned element from the top of the stack
  subroutine pop_(this, item)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    type(BUD_FSTACK_TYPE), intent(inout) :: item

    integer :: el

    if ( .not. is_initd(this) ) then
      call delete(item)
      return
    end if

    el = size(this)

    ! easy case if it is empty
    if ( 0 == el ) then

      call delete(item)
      return

    end if

    ! retrieve the latest value
    item = this%D%T(el)

    call pop(this)

  end subroutine pop_


  !> @param[inout] this stack that gets removed an item
  !! @param[in] top @opt=1 number of popped elements from top.
  !! @param[in] bottom @opt=@null has precedence if `top` not supplied.
  !!                Pops `bot` elements from the bottom of the stack.
  subroutine pop_delete_(this, top, bottom)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer, intent(in), optional :: top, bottom

    integer :: lnpop
    integer :: el, i

    ! it is already empty...
    if ( .not. is_initd(this) ) return

    if ( .not. present(top) &
      .and. .not. present(bottom) ) then
      call set_error(this, 1)
      return
    end if

    ! number of elements
    el = size(this)

    if ( present(top) .or. .not. present(bottom) ) then

      ! regular top-popping
      lnpop = 1

      if ( present(top) ) lnpop = top

      ! we should not pop anything
      if ( lnpop < 0 ) return

      do while ( lnpop > 0 )

        ! easy case if it is empty
        if ( 0 == el ) return

        ! Delete the local item and decrease counter
        call delete(this%D%T(el))
        this%D%nel = this%D%nel - 1

        el = el - 1
        lnpop = lnpop - 1
      end do

    else ! bottom has been supplied

      ! bottom popping
      lnpop = bottom

      ! in case of a negative number we cannot pop anything
      if ( lnpop < 0 ) return

      do while ( lnpop > 0 )

        ! easy case if it is empty
        if ( 0 == el ) return

        ! Delete the local item and decrease counter
        call delete(this%D%T(1))
        this%D%nel = this%D%nel - 1
        ! shift to attain the correct order
        do i = 1 , this%D%nel
          this%D%T(i) = this%D%T(i+1)
        end do

        el = el - 1
        lnpop = lnpop - 1
      end do

    end if

  end subroutine pop_delete_

  !> @param[inout] this stack to clear all elements of
  subroutine clear_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    integer :: el

    do el = size(this), 1, -1

      call pop(this, el)

    end do

  end subroutine clear_

  !> @param[inout] this stack to operate on
  !! @param[in] top @opt=1 number of elements kept from the top
  !! @param[in] bottom @opt=@null number of elements kept from the bottom,
  !!                only used if `top` is not supplied.
  subroutine retain_(this, top, bottom)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer, intent(in), optional :: top, bottom

    integer :: lnpop
    integer :: el

    if ( .not. is_initd(this) ) return

    if ( .not. present(top) &
      .and. .not. present(bottom) ) then
      call set_error(this, 1)
      return
    end if

    ! number of elements
    el = size(this)

    if ( present(top) .or. .not. present(bottom) ) then

      ! regular bot-popping
      lnpop = 1
      if ( present(top) ) lnpop = el - top

      ! we should not pop anything
      if ( lnpop < 0 ) return

      call pop(this, bottom=lnpop)

    else

      ! retain 'n' items from the bottom
      lnpop = el - bottom

      ! we should not pop anything
      if ( lnpop < 0 ) return

      call pop(this, top=lnpop)

    end if

  end subroutine retain_


  !> @param[in] this stack to retrieve element from
  !! @param[inout] elem returned item stored in element `iel`
  !! @param[in] iel @opt=`size(this)` index of element to return. May be negative to offset from the top
  subroutine get_elem_(this, elem, iel)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    type(BUD_FSTACK_TYPE), intent(inout) :: elem
    integer, intent(in), optional :: iel
    integer :: el

    call delete(elem)
    if ( .not. is_initd(this) ) return

    ! Default element
    el = size(this)

    if ( present(iel) ) then
      if ( iel < 0 ) then
        el = el + iel
      else
        el = iel
      end if
    end if

    if ( el <= size(this) ) then
      elem = this%D%T(el)
    end if

  end subroutine get_elem_

  !> @param[in] this stack to retrieve pointer element from
  !! @param[in] iel @opt=`size(this)` index of element to return. May be negative to offset from top.
  !! @result pointer to the element `iel`
  function get_elem_p_(this, iel) result(p)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer, intent(in), optional :: iel
    type(BUD_FSTACK_TYPE), pointer :: p
    integer :: el

    nullify(p)
    if ( .not. is_initd(this) ) return

    ! Default element
    el = size(this)

    if ( present(iel) ) then
      if ( iel < 0 ) then
        el = el + iel
      else
        el = iel
      end if
    end if

    if ( el <= size(this) ) then
      p => this%D%T(el)
    end if

  end function get_elem_p_


  !> @param[inout] this stack to count element from
  !! @return number of currently stored elements
  pure function size_elem_(this) result (nel)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: nel
    if ( .not. is_initd(this) ) then
      nel = 0
    else
      nel = this%D%nel
    end if
  end function size_elem_


  !> @param[inout] this stack that is queried maximum size of
  !! @return maximum number of allowed elements
  pure function size_max_elem_(this) result (nel_max)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: nel_max
    if ( .not. is_initd(this) ) then
      nel_max = 0
    else
      nel_max = this%D%nel_max
    end if
  end function size_max_elem_


#ifndef BUD_NO_IO
# ifndef BUD_IO_OVERLOAD

  !> @param[in] f a file (bud_File)
  !! @param[in] this collection @bud to be written
  subroutine write_(f, this)
    use BUD_CC2(BUD_MOD,_File)
    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

    logical :: formatted
    integer :: iu
    integer :: nel, nel_max, i

    ! If file is not opened, return immediately
    if ( .not. is_open(f) ) return
    if ( .not. is_initd(this) ) return

    formatted = is_formatted(f)
    iu = unit(f)

    ! Get information about this stack
    nel_max = size_max(this)
    nel = size(this)

    if ( formatted ) then
      write(iu, '(i16)') nel_max, nel
    else
      write(iu) nel_max, nel
    end if

    ! Now write each of them...
    do i = 1 , nel
      call write(f, this%D%T(i))
    end do

  end subroutine write_

  !> @param[in] f a file (bud_File)
  !! @param[in] this collection @bud to be readed
  subroutine read_(f, this)
    use BUD_CC2(BUD_MOD,_File)
    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    ! To read each of them individually
    type(BUD_FSTACK_TYPE) :: cp

    logical :: formatted
    integer :: iu
    integer :: nel, nel_max, i

    ! If file is not opened, return immediately
    if ( .not. is_open(f) ) return

    formatted = is_formatted(f)
    iu = unit(f)

    ! Get information about this stack
    if ( formatted ) then
      read(iu, '(i16)') nel_max, nel
    else
      read(iu) nel_max, nel
    end if

    ! Initialize this object
    call new(this, nel_max)

    ! Now read each of them...
    do i = 1 , nel
      call read(f, cp)
      call push(this, cp)
      call delete(cp)
    end do

  end subroutine read_

# endif
#endif

  !> @param[in] this data type
  !! @param[in] info @opt=BUD_TYPE_NAME_STR additional information printed
  !! @param[in] indent @opt=1 possible indentation of printed statement
  subroutine print_(this, info, indent)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    character(len=*), intent(in), optional :: info
    integer, intent(in), optional :: indent

    integer :: lindent
    integer :: i

    ! 4-byte variable
    character(len=32) :: fmt
    character(len=256) :: name

    name = BUD_TYPE_NAME_STR
    if ( present(info) ) name = info
    lindent = 1
    if ( present(indent) ) lindent = indent

    write(fmt, '(a,i0,a)') '(t',lindent,',3a)'

    if ( .not. is_initd(this) ) then
      write(*,fmt) "<", trim(name), " not initialized>"
      return
    end if

    write(fmt, '(a,i0,a)') '(t',lindent,',3a)'
    lindent = lindent + 2 ! step indentation
    write(*,fmt) "<<", trim(name), " stack>"
    do i = 1 , size(this)
      call print(this%D%T(i), indent = lindent)
    end do
    lindent = lindent - 2 ! go back to requested indentation
    write(fmt, '(a,i0,a)') '(t',lindent,',a,i0,a)'
    write(*,fmt) " <stack-refs: ", references(this), ">>"

  end subroutine print_


  ! project-buds -- local file settings
  !     Anything below this line may be overwritten by scripts
  !     Below are non-editable settings

  ! Local Variables:
  !  mode: f90
  !  f90-if-indent: 2
  !  f90-type-indent: 2
  !  f90-associate-indent: 2
  !  f90-continuation-indent: 2
  !  f90-structure-indent: 2
  !  f90-critical-indent: 2
  !  f90-program-indent: 2
  !  f90-do-indent: 2
  ! End:
