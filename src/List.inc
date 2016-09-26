  ! @@LICENSE@@ see Copyright notice in the top-directory
  
#include "bud_utils.inc"
  
  ! The following pre-processor variables are currently used when
  ! included:
  !  - BUD_MOD_NAME of the module
  !  - BUD_TYPE_NAME of the type
  !  - BUD_TYPE_NAME_, internal data pointer of the type
  !  - BUD_TYPE_NAME_STR, the routine name in "string" format (for IO)
  !  - BUD_TYPE_NEW, the routine name for creating a new type
  !  - BUD_TYPE_VAR, the variable type contained in the array
  !  - BUD_TYPE_VAR_PREC, the precision specifier for the array


#define BUD_MOD_NAME_STR BUD_XSTR(BUD_MOD_NAME)
#define BUD_TYPE_NAME_ BUD_CC2(BUD_TYPE_NAME,_)
#define BUD_TYPE_NAME_STR BUD_XSTR(BUD_TYPE_NAME)
  
  !> BUD_MOD_NAME documentation
  !!
  !! @details
  !! The detailed implementation of the BUD_TYPE_NAME type.
  !!
  !! @note
  !! This documentation is duplicated in all types with different precision.

  ! This *MUST* be the first statement
  ! Common parameters and variables used
# include "bud_common_declarations.inc"

  !> Maximum name length of the list
  integer, parameter :: LIST_MAX_NAME_LEN = 64

  !> List data type
  !!
  !! This data type contains the reference counted
  !! List object.
  !!
  !! The content is of fixed size. So changing the
  !! List dimensions is heavy on performance.
  type BUD_TYPE_NAME

    !> @cond BUD_DEVELOPER
    
    !> Stored pointer which contains the reference counting etc.
    type(BUD_TYPE_NAME_), pointer :: D => null()

    !> @endcond BUD_DEVELOPER

#if BUD_FORTRAN >= 2003
  contains
#   include "bud_common_type.inc"

    !> @name Private procedures
    !> @{
    ! Doxygen needed line
    
    procedure, private :: new_dim_
    procedure, private :: new_copy_
    procedure, private :: new_copy_n_
    procedure, private :: new_range_

    !> @}
    
    !> @iSee BUD_NEW_TYPE
    generic, public :: BUD_NEW_TYPE => new_dim_, new_copy_, &
      new_copy_n_, new_range_
    !> @iSee BUD_NEW_TYPE
    generic, public :: new => new_dim_, new_copy_, &
      new_copy_n_, new_range_

    !> @iSee #size
    procedure, public :: size => size_
    !> @iSee #sizep
    procedure, public :: sizep => sizep_

    !> @iSee #max_size
    procedure, public :: max_size => max_size_
    !> @iSee #max_sizep
    procedure, public :: max_sizep => max_sizep_

    !> @iSee #listp
    procedure, public :: listp => listp_
    !> @iSee #max_listp
    procedure, public :: list_maxp => list_maxp_

    !> @iSee #sort_heap
    procedure, public :: list_sort => sort_heap_

    !> @iSee #list_push
    generic, public :: list_push => push_value_, push_array_, &
      push_list_

    
    ! Set operations
    !> @iSee #list_union
    generic, public :: list_union => union_
    !> @iSee #list_complement
    generic, public :: list_complement => complement_
    !> @iSee #list_intersect
    generic, public :: list_intersect => intersect_
    

    !> @iSee #list_reverse
    generic, public :: list_reverse => reverse_

    !> @iSee #list_extend
    procedure, public :: list_extend => extend_

    !> @iSee #index
    procedure, public :: index => index_

    !> @iSee #in_list
    procedure, public :: in_list => in_list_

    !> @iSee #is_sorted
    procedure, public :: is_sorted => is_sorted_

#endif
  end type BUD_TYPE_NAME

  !> @cond BUD_DEVELOPER
  
  !> @bud container for BUD_TYPE_NAME
  !!
  !! Contains a List of integers.
  !!
  type BUD_TYPE_NAME_

    !> Associated name of the list
    character(len=LIST_MAX_NAME_LEN) :: name = ' '

    !> Current number of stored values
    integer(BUD_TYPE_VAR_PREC) :: n = 0

    !> Current maximum number of values
    integer(BUD_TYPE_VAR_PREC) :: max_n = 0
    
    !> Incremental extension when the list is not enough
    !! In some cases this may be advantageous to be a relatively low number.
    integer(BUD_TYPE_VAR_PREC) :: incr_n = 10

    !> List content stored in the @bud
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: lst(:) => null()

    !> Whether this list is a sorted list, or not
    !!
    !! A sorted list has certain efficient properties
    !! when querying certain elements.
    logical :: sorted = .false.
    
    ! Consistent data in the reference counted object
#   include "bud_common_type_.inc"

  end type BUD_TYPE_NAME_

  !> @endcond BUD_DEVELOPER

  !> Create a new BUD_TYPE_NAME.
  !!
  !! This allocates the data list in the @bud.
  !!
  !! The dimensions of the contained array may be
  !! explicitly passed, or an array may be passed
  !! which will be copied to the contained array.
  !!
  !! @note
  !! If the array is created from dimensions, the initial
  !! value of the array is arbitrary.
  interface BUD_TYPE_NEW
    module procedure new_dim_
    module procedure new_copy_
    module procedure new_copy_n_
    module procedure new_range_
  end interface
  public :: BUD_TYPE_NEW
  
  !> @iSee BUD_TYPE_NEW
  interface new
    module procedure new_dim_
    module procedure new_copy_
    module procedure new_copy_n_
    module procedure new_range_
  end interface
  public :: new


  !> Retrieve pointer to array in the @bud
  !!
  !! The array-pointer will _always_ be contiguous.
  !!
  !! This pointer will only be a sub-set pointer of the
  !! total list array if `size(this) < max_size(this)`.
  !! @iSee #max_listp for the full list pointer.
  !!
  !! @note
  !! Do not deallocate the array-pointer.
  interface listp
    module procedure listp_
  end interface
  public :: listp

  !> Retrieve pointer to full array in the @bud
  !!
  !! The array-pointer will _always_ be contiguous.
  !!
  !! @note
  !! Do not deallocate the array-pointer.
  interface list_maxp
    module procedure max_listp_
  end interface
  public :: list_maxp


  !> Query current size of list (number of current elements)
  !!
  !! Returns the size of the list.
  !!
  !! This is equivalent to:
  !! \code{.f90}
  !! a => listp(this)
  !! ... = size(a)
  !! \endcode
  interface size
    module procedure size_
  end interface
  public :: size

  
  !> Track the current size of list (number of current elements)
  !!
  !! Returns a pointer to the size of the list.
  interface sizep
    module procedure sizep_
  end interface
  public :: sizep


  !> Query maximum allowed size of list
  !!
  !! Returns the allowed size of the list.
  interface max_size
    module procedure max_size_
  end interface
  public :: max_size

  !> Track maximum allowed size of list
  !!
  !! Returns a pointer to the the allowed size of the list.
  interface max_sizep
    module procedure max_sizep_
  end interface
  public :: max_sizep

  !> Returns index of the queried element. If non-existing returns `-1`.
  interface index
    module procedure index_
  end interface
  public :: index


  !> Push a new value to the list (added at the end)
  !!
  !! Will append a new value in the list (at the end).
  !! In case there is not any available elements, i.e. if:
  !! \code{.f90}
  !! size(this) == max_size(this)
  !! \endcode
  !! then the internal array will be re-allocated.
  !! This means that a pointer to the array should be
  !! re-instantiated, if needed.
  interface list_push
    module procedure push_value_, push_array_, push_list_
  end interface list_push

  !> Sorts the entries in the list
  !!
  !! The values in the list will be sorted.
  !!
  !! To create a copy of the list and sort the copied list
  !! one must do the following:
  !! \code{.f90}
  !!   call copy(this, other)
  !!   call list_sort(other)
  !! \endcode
  interface list_sort
    module procedure sort_
  end interface list_sort


  !> Ensures that the list may contain a given number of values
  !!
  !! One may query an explicit number of maximum values (`max_n`),
  !! or an additive number of values (`n`).
  interface list_extend
    module procedure extend_
  end interface list_extend


  !> Returns the union of two lists in a third list
  interface list_union
    module procedure union_
  end interface list_union

  !> Returns the intersection of two lists in a third list
  interface list_intersect
    module procedure intersect_
  end interface list_intersect

  !> Returns the complement of two lists in a third list
  interface list_complement
    module procedure complement_
  end interface list_complement

  !> Reverses the list
  interface list_reverse
    module procedure reverse_
  end interface list_reverse


  !> Query whether a value is in the list
  interface in_list
    module procedure in_list_
  end interface in_list

  !> Query whether the list is sorted. It will return `.true.` if the list has
  !! size == 1, or if it has been sorted.
  interface is_sorted
    module procedure is_sorted_
  end interface is_sorted

  
  !> Print, to std-out, some basic information of the data-container
  !!
  !! Print out XML-like information regarding the data-container.
  !!
  !! @wanted
  !! Retrieval function of the string that represents the data.
  !! This will enable the parent program to show it in the way it wants.
  interface print
    module procedure print_
  end interface
  public :: print

  
  ! Include common data routines
  ! Note that 'CONTAINS' is present in this include file.
# include "bud_common.inc"


  !> @cond BUD_DEVELOPER
  
  !> Internal routine for cleaning up the data container.
  !!
  !! @dev_note
  !! This routine is only used internally to clean-up
  !! any data in the type.
  !! Should never be made public.
  pure subroutine common_delete_(this)
    type(BUD_TYPE_NAME_), intent(inout) :: this

    ! Currently we do not allow external memory
    ! tracking.
    if ( .not. associated(this%lst) ) return
    deallocate(this%lst)
    nullify(this%lst)
    this%n = 0
    this%max_n = 0
    this%incr_n = 10
    this%sorted = .false.
    
  end subroutine common_delete_

  !> @endcond BUD_DEVELOPER

  
  !> @param[in] from the origin of data
  !! @param[inout] to the destination of the data
  subroutine copy_(from, to)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    call delete(to)
    if ( .not. is_initd(from) ) return

    call new_copy_n_(to, from%D%n, from%D%lst)

    call common_copy_(from, to)
    
  end subroutine copy_
  

  !> @param[inout] this array @bud
  !! @param[in] n size of list
  subroutine new_dim_(this, n)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(BUD_TYPE_VAR_PREC), intent(in) :: n

    ! Make sure the container is initialized and ready for
    ! data-creation.

    call initialize(this)

    ! Allocate the value array
    allocate(this%D%lst(n))
    this%D%max_n = n
    this%D%n = 0
    this%D%incr_n = 10
    this%D%sorted = .false.

  end subroutine new_dim_


  !> @param[inout] this list @bud
  !! @param[in] list list to dublicate in `this` (dimensions _and_ values)
  subroutine new_copy_(this, list)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: list(:)

    integer(BUD_TYPE_VAR_PREC) :: n

    n = size(list,dim=1)
    
    call new_copy_n_(this, n, list)

  end subroutine new_copy_

  
  !> @param[inout] this array @bud
  !! @param[in] a first value of the range
  !! @param[in] b last value of the range
  !! @param[in] step @optional=1 the step of the sequence
  subroutine new_range_(this, a, b, step)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: a, b
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in), optional :: step

    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC) :: c, lstep
    integer(BUD_TYPE_VAR_PREC) :: i, n

    lstep = 1
    if ( present(step) ) lstep = step
    
    n = (b - a) / lstep
    call new_dim_(this, n)

    this%D%n = this%D%max_n
    c = a
    do i = 1 , n
      this%D%lst(i) = c
      c = c + step
    end do
    
  end subroutine new_range_

  !> @param[inout] this list @bud
  !! @param[in] n size of list
  !! @param[in] list list to dublicate in `this` (dimensions _and_ values)
  subroutine new_copy_n_(this, n, list)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(BUD_TYPE_VAR_PREC), intent(in) :: n
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: list(n)

    ! initialize and create data-container
    call new_dim_(this, n)

    ! copy data
    call list_push(this, n, list)

  end subroutine new_copy_n_


  !> @param[in] this @bud container
  !! @return the size of the contained list
  pure function size_(this) result(d)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

    integer(BUD_TYPE_VAR_PREC) :: d

    if ( is_initd(this) ) then
      d = this%D%n
    else
      d = 0
    end if

  end function size_

  !> @param[in] this @bud container
  !! @return a pointer to the current size of the contained list
  function sizep_(this) result(d)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

    integer(BUD_TYPE_VAR_PREC), pointer :: d

    if ( is_initd(this) ) then
      d => this%D%n
    else
      nullify(d)
    end if

  end function sizep_


  !> @param[in] this @bud container
  !! @return the size of the contained list
  pure function max_size_(this) result(d)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

    integer(BUD_TYPE_VAR_PREC) :: d

    if ( is_initd(this) ) then
      d = this%D%max_n
    else
      d = 0
    end if

  end function max_size_


  !> @param[in] this @bud container
  !! @return the size of the contained list
  function max_sizep_(this) result(d)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

    integer(BUD_TYPE_VAR_PREC), pointer :: d

    if ( is_initd(this) ) then
      d => this%D%max_n
    else
      nullify(d)
    end if

  end function max_sizep_


  !> @param[in] this list
  !! @param[in] incr_n the default number of values that should be

  !> @cond BUD_DEVELOPER
  
  !> Initialize `bud` if not initialized with size `n`, else extend by `n`
  !!
  !! If `this` is not initialized the `bud` will be initialized and
  !! have initial size `n`.
  !! Else, `this` will be extended to be at least capable of retaining
  !! its current elements + `n`.
  !!
  !! @param[inout] this List to be asserted
  !! @param[in] n size to be initialized/extended
  subroutine assert_init_size_add(this, n)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(BUD_TYPE_VAR_PREC), intent(in) :: n

    if ( is_initd(this) ) then
      
      if ( this%D%max_n < this%D%n + n ) then
        call list_extend(this, max_n= &
          max(this%D%n+n,this%D%max_n+this%D%incr_n))
      end if

    else
      
      call new_dim_(this, n)
      
    end if
    
  end subroutine assert_init_size_add

  !> @endcond BUD_DEVELOPER


  !> @param[inout] this list @bud
  !! @param[in] val value to be appended to the list
  subroutine push_value_(this, val)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC) :: val

    call assert_init_size_add(this, BUD_CC3(1,_,BUD_TYPE_VAR_PREC))

    this%D%n = this%D%n + 1
    this%D%lst(this%D%n) = val

    if ( this%D%sorted .and. this%D%n > 1 ) then
      this%D%sorted = this%D%lst(this%D%n-1) <= this%D%lst(this%D%n)
    else
      this%D%sorted = .false.
    end if

  end subroutine push_value_


  !> @param[inout] this list @bud
  !! @param[in] n size of list to be added to the list
  !! @param[in] list the list to be added to the list
  subroutine push_array_(this, n, list)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC) :: n, list(n)
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC) :: i
    logical :: sorted
    
    call assert_init_size_add(this, n)

    if ( this%D%n > 0 ) then
      sorted = this%D%lst(this%D%n) <= list(1)
    else
      ! This is an empty list, so
      ! it will probably be default to
      ! true if the passed list is sorted.
      sorted = .true.
    end if

    this%D%lst(this%D%n+1) = list(1)
    do i = 2 , n
      this%D%lst(this%D%n+i) = list(i)
      sorted = sorted .and. &
        list(i-1) <= list(i)
    end do
    
    this%D%n = this%D%n + n

    this%D%sorted = this%D%sorted .and. sorted

  end subroutine push_array_

  !> @param[inout] this list @bud
  !! @param[in] other other list to be added to `this`
  subroutine push_list_(this, other)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: other

    if ( .not. is_initd(other) ) return

    call list_push(this, other%D%n, other%D%lst)
    
  end subroutine push_list_


  !> @param[in] this list @bud
  !! @return a pointer to the list (contiguous)
  function listp_(this) result(p)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: p(:)
#else
    BUD_TYPE_VAR, pointer BUD_FORTRAN_CONTIGUOUS :: p(:)
#endif

    ! only return the currently known values
    p => this%D%lst(1:this%D%n)

  end function listp_

  !> @param[in] this list @bud
  !! @return a pointer to the list (contiguous)
  function max_listp_(this) result(p)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: p(:)
#else
    BUD_TYPE_VAR, pointer BUD_FORTRAN_CONTIGUOUS :: p(:)
#endif

    ! only return the currently known values
    p => this%D%lst
    
  end function max_listp_

  
  !> @param[in] this @bud container
  !! @param[in] max_n maximum number of allowed elements that should be available in the list
  !! @param[in] n @opt number of elements that needs to be available in its current state
  subroutine extend_(this, max_n, n)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), optional :: max_n, n

    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC) :: lmax_n, i
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), allocatable :: tmp(:)

    ! We will select the larger of the two
    !   max(max_n, this%D%n + n, this%D%max_n)
    if ( present(n) .and. present(max_n) ) then
      lmax_n = max(max_n, this%D%max_n, this%D%n + n)
    else if ( present(n) ) then
      lmax_n = max(this%D%max_n, this%D%n + n)
    else if ( present(max_n) ) then
      lmax_n = max(this%D%max_n, max_n)
    else

      ! The user is actually not requesting any
      ! change of size
      return

    end if

    ! quick-return if possible...
    if ( lmax_n <= this%D%max_n ) return

    ! Extend the array to the new size
    allocate(tmp(this%D%n))
    do i = 1, this%D%n
      tmp(i) = this%D%lst(i)
    end do
    
    deallocate(this%D%lst)
    nullify(this%D%lst)
    allocate(this%D%lst(this%D%n+n))
    ! set the new maximum size of the list
    this%D%max_n = lmax_n
    do i = 1, this%D%n
      this%D%lst(i) = tmp(i)
    end do
    deallocate(tmp)

  end subroutine extend_


  !> @param[inout] this list @bud (sorted on exit)
  subroutine sort_(this)
    use BUD_CC2(BUD_MOD,_utils), only: sort_heap
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    
    call sort_heap(this%D%n, this%D%lst)

    this%D%sorted = .true.
    
  end subroutine sort_


  !> @param[in] A list one
  !> @param[in] B list two
  !> @param[inout] C the list that intersects `A` and `B`
  subroutine intersect_(A, B, C)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: A, B
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: C

    BUD_CLASS(BUD_TYPE_NAME) :: D
    integer(BUD_TYPE_VAR_PREC) :: i, n
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: l(:)

    ! Ensure the output list is empty
    call delete(C)

    ! Quick return if the list is
    n = min(size(A), size(B))
    if ( n == 0 ) return

    ! Prepare list `C`
    call new_dim_(C, n)

    ! Prefer to not copy any of the sets...
    if ( is_sorted(A) .and. .not. is_sorted(B) ) then

      ! we search in A
      l = listp(B)
      do i = 1 , size(B)
        if ( in_list(A, l(i)) ) then
          call list_push(C, l(i))
        end if
      end do

    else if ( .not. is_sorted(A) .and. is_sorted(B) ) then

      ! we search in B
      l = listp(A)
      do i = 1 , size(A)
        if ( in_list(B, l(i)) ) then
          call list_push(C, l(i))
        end if
      end do

    else if ( size(A) > size(B) ) then

      ! We will sort list B and then perform the list insertion
      ! We will sort list B and then perform the list insertion
      if ( is_sorted(B) ) then
        ! both are actually sorted, but B is smaller
        D = B
      else
        call copy(B, D)
        call list_sort(D)
      end if

      ! we search in B
      l = listp(A)
      do i = 1 , size(A)
        if ( in_list(D, l(i)) ) then
          call list_push(C, l(i))
        end if
      end do

      call delete(D)
      
    else

      ! We will sort list B and then perform the list insertion
      if ( is_sorted(A) ) then
        ! both are actually sorted, but B is smaller
        D = A
      else
        call copy(A, D)
        call list_sort(D)
      end if

      ! we search in A
      l = listp(B)
      do i = 1 , size(B)
        if ( in_list(D, l(i)) ) then
          call list_push(C, l(i))
        end if
      end do

      call delete(D)

    end if
      
  end subroutine intersect_


  !> @param[in] A list one
  !> @param[in] B list two
  !> @param[inout] C list as union of `A` and `B`
  subroutine union_(A, B, C)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: A, B
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: C

    BUD_CLASS(BUD_TYPE_NAME) :: D
    integer(BUD_TYPE_VAR_PREC) :: i, n
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: l(:)

    if ( size(A) == 0 ) then
      call copy(B, C)
      return
    else if ( size(B) == 0 ) then
      call copy(A, C)
      return
    end if

    ! Get maximum size of the union and allocate
    n = size(A) + size(B)
    call new_dim_(C, n)
    
    ! Prefer to not copy any of the sets...
    if ( is_sorted(A) .and. .not. is_sorted(B) ) then

      ! First populate A
      call list_push(C, A)
      
      ! we search in A
      l = listp(B)
      do i = 1 , size(B)
        if ( .not. in_list(A, l(i)) ) then
          call list_push(C, l(i))
        end if
      end do

    else if ( .not. is_sorted(A) .and. is_sorted(B) ) then
      
      ! First populate B
      call list_push(C, B)

      ! we search in B
      l = listp(A)
      do i = 1 , size(A)
        if ( .not. in_list(B, l(i)) ) then
          call list_push(C, l(i))
        end if
      end do

    else if ( size(A) > size(B) ) then

      ! We will sort list B and then perform the list insertion
      if ( is_sorted(B) ) then
        ! both are actually sorted, but B is smaller
        D = B
      else
        call copy(B, D)
        call list_sort(D)
      end if

      ! First populate B
      call list_push(C, B)

      ! we search in B
      l = listp(A)
      do i = 1 , size(A)
        if ( .not. in_list(D, l(i)) ) then
          call list_push(C, l(i))
        end if
      end do

      call delete(D)
      
    else

      ! We will sort list B and then perform the list insertion
      if ( is_sorted(A) ) then
        ! both are actually sorted, but B is smaller
        D = A
      else
        call copy(A, D)
        call list_sort(D)
      end if

      ! First populate A
      call list_push(C, A)

      ! we search in A
      l = listp(B)
      do i = 1 , size(B)
        if ( .not. in_list(D, l(i)) ) then
          call list_push(C, l(i))
        end if
      end do

      call delete(D)

    end if
      
  end subroutine union_


  !> @param[in] A list one
  !> @param[in] B list two
  !> @param[inout] C list as the complement: `A \ B`, i.e. elements in `B` but not in `A`.
  subroutine complement_(A, B, C)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: A, B
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: C

    BUD_CLASS(BUD_TYPE_NAME) :: D
    integer(BUD_TYPE_VAR_PREC) :: i, n
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: l(:)

    if ( size(A) == 0 ) then
      call copy(B, C)
      return
    else if ( size(B) == 0 ) then
      call copy(A, C)
      return
    end if

    ! Get maximum size of the union and allocate
    n = size(B)
    call new_dim_(C, n)

    if ( is_sorted(A) ) then
      D = A
    else
      call copy(A, D)
      call list_sort(D)
    end if
      
    ! This is easy...
    
    ! we search in A
    l = listp(B)
    do i = 1 , size(B)
      if ( .not. in_list(D, l(i)) ) then
        call list_push(C, l(i))
      end if
    end do

    call delete(D)

  end subroutine complement_


  !> @param[inout] this the list which will be returned with reversed elements
  subroutine reverse_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(BUD_TYPE_VAR_PREC) :: i, n
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: l(:)
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC) :: tmp

    n = size(this)
    l => listp(this)
    
    do i = 1 , n / 2
      tmp = l(i)
      l(i) = l(n+1-i)
      l(n+1-i) = tmp
    end do
    
  end subroutine reverse_


  
  !> @param[inout] this list @bud
  !! @param[in] val the value to be indexed in the list
  !! @return the index of `val` in the list (0 if not found)
  pure function index_(this, val) result(idx)
    use BUD_CC2(BUD_MOD,_utils), only: find_bin
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: val
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC) :: idx

    if ( this%D%sorted ) then
      
      ! we may easily find indices of a sorted array
      call find_bin(this%D%n, this%D%lst, val, idx)

      return

    else

      do idx = 1 , this%D%n
        if ( this%D%lst(idx) == val ) return
      end do
      
    end if
    
    idx = 0

  end function index_

  !> @param[in] this list @bud
  !! @param[in] val the value to be queried whether it is in the list
  !! @return .true. if `val` is in the list
  pure function in_list_(this, val) result(found)
    use BUD_CC2(BUD_MOD,_utils), only: find_bin
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: val
    logical :: found

    found = index(this, val) > 0

  end function in_list_

  
  !> @param[in] this list @bud
  !! @return .true. if `this` is a sorted list
  pure function is_sorted_(this) result(sorted)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    logical :: sorted

    sorted = this%D%sorted
    if ( .not. sorted ) then
      sorted = size(this) == 1
    end if
    
  end function is_sorted_



  !> @param[in] this list @bud
  !! @param[in] info @opt=BUD_TYPE_NAME_STR additional information printed
  !! @param[in] indent @opt=1 possible indentation of printed statement
  subroutine print_(this, info, indent)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    character(len=*), intent(in), optional :: info
    integer, intent(in), optional :: indent

    integer :: lindent
    integer(BUD_TYPE_VAR_PREC) :: n, max_n

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

    ! Get size
    n = size_(this)
    max_n = max_size_(this)
    
    ! Create fmt
    write(fmt, '(a,i0,a)') '(t',lindent,',3a,10(i0,a))'
    
    write(*,fmt) "<", trim(name), " size/max=", n,'/',max_n, &
      ", refs: ", references(this), ">"

  end subroutine print_

  
  ! Local pre-processor variables that
  ! undefine the variables that are not needed anymore.
#undef BUD_TYPE_NEW
#undef BUD_TYPE_VAR
#undef BUD_TYPE_VAR_PREC

#include "bud_cleanup.inc"
  
  
! project-bud -- local file settings
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
