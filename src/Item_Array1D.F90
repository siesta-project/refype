  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

! Integer (short)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_Item_,sArray1D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,sArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,Item_,sArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_Item_,sArray1D)
# define BUD_ITEM_TYPE BUD_CC2(BUD_TYPE,sArray1D)
#include "bud_item.inc"
end module

! Integer (int)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_Item_,iArray1D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,iArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,Item_,iArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_Item_,iArray1D)
# define BUD_ITEM_TYPE BUD_CC2(BUD_TYPE,iArray1D)
#include "bud_item.inc"
end module

! Integer (long)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_Item_,lArray1D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,lArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,Item_,lArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_Item_,lArray1D)
# define BUD_ITEM_TYPE BUD_CC2(BUD_TYPE,lArray1D)
#include "bud_item.inc"
end module

! Real (single)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_Item_,rArray1D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,rArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,Item_,rArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_Item_,rArray1D)
# define BUD_ITEM_TYPE BUD_CC2(BUD_TYPE,rArray1D)
#include "bud_item.inc"
end module

! Real (double)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_Item_,dArray1D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,dArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,Item_,dArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_Item_,dArray1D)
# define BUD_ITEM_TYPE BUD_CC2(BUD_TYPE,dArray1D)
#include "bud_item.inc"
end module

! Complex (single)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_Item_,cArray1D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,cArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,Item_,cArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_Item_,cArray1D)
# define BUD_ITEM_TYPE BUD_CC2(BUD_TYPE,cArray1D)
#include "bud_item.inc"
end module

! Complex (double)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_Item_,zArray1D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,zArray1D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,Item_,zArray1D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_Item_,zArray1D)
# define BUD_ITEM_TYPE BUD_CC2(BUD_TYPE,zArray1D)
#include "bud_item.inc"
end module

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
