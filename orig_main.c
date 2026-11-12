// Lifted from the original 0x0800F714-0x0800F884 by tools/lift.py: exact, not yet readable.
typedef unsigned char u8; typedef signed char s8; typedef unsigned short u16; typedef short s16;
typedef unsigned int u32; typedef int s32;
extern char hook_render[];
extern char hook_update[];
extern char o_build_palette[];
extern char o_game_init[];
extern char o_rng_mix[];
void o_main(void){
  u32 r0=0,r1=0,r2=0,r3=0,r4=0,r5=0,r6=0,r7=0,r8=0,r9=0,r10=0,r11=0,r12=0,lr=0; u32 N=0,Z=0,C=0,V=0;
  u32 frame_[160]; u32 sp=(u32)&frame_[160];
  (void)r8;(void)r9;(void)r10;(void)r11;(void)r12;(void)lr;(void)V;(void)N;(void)Z;(void)C;
L_0800f714: ;  sp-=8; *(volatile u32*)(sp+0)=r7; *(volatile u32*)(sp+4)=lr;                                         /* 0f714 push {r7, lr} */
               { u32 X_=(u32)(sp), Y_=(u32)(0u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; r7=R_; } /* 0f716 add r7, sp, #0 */
               { u32 X_=(u32)(sp), Y_=(~(u32)(16u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; sp=R_; } /* 0f718 sub sp, #0x10 */
               r0=0x04000200u;                                                                                      /* 0f71a ldr r0, [pc, #0x140] */
               r1=0x00004317u;                                                                                      /* 0f71c ldr r1, [pc, #0x140] */
               *(volatile u16*)((r0+4))=(u16)r1;                                                                    /* 0f71e strh r1, [r0, #4] */
               r5=1u; N=(r5)>>31; Z=(r5)==0;                                                                        /* 0f720 movs r5, #1 */
               { u32 X_=(u32)(r5), S_=(u32)(26u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r6=R_; N=R_>>31; Z=R_==0; } /* 0f722 lsls r6, r5, #0x1a */
               r1=128u; N=(r1)>>31; Z=(r1)==0;                                                                      /* 0f724 movs r1, #0x80 */
               *(volatile u16*)(r6)=(u16)r1;                                                                        /* 0f726 strh r1, [r6] */
               r2=0x04000080u;                                                                                      /* 0f728 ldr r2, [pc, #0x138] */
               *(volatile u16*)((r2+4))=(u16)r1;                                                                    /* 0f72a strh r1, [r2, #4] */
               r3=0x00003377u;                                                                                      /* 0f72c ldr r3, [pc, #0x138] */
               *(volatile u16*)(r2)=(u16)r3;                                                                        /* 0f72e strh r3, [r2] */
               r3=2u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 0f730 movs r3, #2 */
               *(volatile u16*)((r2+2))=(u16)r3;                                                                    /* 0f732 strh r3, [r2, #2] */
               r2=0x04000100u;                                                                                      /* 0f734 ldr r2, [pc, #0x134] */
               r4=0u; N=(r4)>>31; Z=(r4)==0;                                                                        /* 0f736 movs r4, #0 */
               *(volatile u16*)(r2)=(u16)r4;                                                                        /* 0f738 strh r4, [r2] */
               *(volatile u16*)((r2+2))=(u16)r1;                                                                    /* 0f73a strh r1, [r2, #2] */
               r1=8u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 0f73c movs r1, #8 */
               *(volatile u32*)((sp+4))=(u32)r6;                                                                    /* 0f73e str r6, [sp, #4] */
               *(volatile u16*)((r6+4))=(u16)r1;                                                                    /* 0f740 strh r1, [r6, #4] */
               *(volatile u16*)(r0)=(u16)r5;                                                                        /* 0f742 strh r5, [r0] */
               *(volatile u16*)((r0+8))=(u16)r5;                                                                    /* 0f744 strh r5, [r0, #8] */
               r0=0x00C0FFBDu;                                                                                      /* 0f746 ldr r0, [pc, #0x128] */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&o_game_init)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* o_game_init */ /* 0f748 bl #0x800ce5c */
               r0=0x0300047Cu;                                                                                      /* 0f74c ldr r0, [pc, #0x124] */
               *(volatile u32*)((sp+8))=(u32)r5;                                                                    /* 0f74e str r5, [sp, #8] */
               *(volatile u32*)(r0)=(u32)r5;                                                                        /* 0f750 str r5, [r0] */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&hook_render)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* hook_render */ /* 0f752 bl #0x8001fb4 */
               r6=0x03000480u;                                                                                      /* 0f756 ldr r6, [pc, #0x120] */
               r5=*(volatile u32*)(r6);                                                                             /* 0f758 ldr r5, [r6] */
L_0800f75a: ;  r0=*(volatile u32*)(r6);                                                                             /* 0f75a ldr r0, [r6] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(r5)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0f75c cmp r0, r5 */
               if(!Z) goto L_0800f764;                                                                              /* 0f75e bne #0x800f764 */
L_0800f760: ;  __asm__ volatile("svc #2" ::: "r0","r1","r2","r3","memory");                                         /* 0f760 svc #2 */
               goto L_0800f75a;                                                                                     /* 0f762 b #0x800f75a */
L_0800f764: ;  r5=5u; N=(r5)>>31; Z=(r5)==0;                                                                        /* 0f764 movs r5, #5 */
               { u32 X_=(u32)(r5), S_=(u32)(24u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r6=R_; N=R_>>31; Z=R_==0; } /* 0f766 lsls r6, r5, #0x18 */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&o_build_palette)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* o_build_palette */ /* 0f768 bl #0x800cc94 */
L_0800f76c: ;  r1=*(volatile u16*)((r0+r4));                                                                        /* 0f76c ldrh r1, [r0, r4] */
               *(volatile u16*)((r6+r4))=(u16)r1;                                                                   /* 0f76e strh r1, [r6, r4] */
               { u32 X_=(u32)(r4), Y_=(u32)(2u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r4=R_; } /* 0f770 adds r4, r4, #2 */
               { u32 X_=(u32)(r4), Y_=(~(u32)(128u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0f772 cmp r4, #0x80 */
               if(!Z) goto L_0800f76c;                                                                              /* 0f774 bne #0x800f76c */
L_0800f776: ;  r0=0x0300047Cu;                                                                                      /* 0f776 ldr r0, [pc, #0xfc] */
               r0=*(volatile u32*)(r0);                                                                             /* 0f778 ldr r0, [r0] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0f77a cmp r0, #0 */
               if(Z) goto L_0800f780;                                                                               /* 0f77c beq #0x800f780 */
L_0800f77e: ;  r5=21u; N=(r5)>>31; Z=(r5)==0;                                                                       /* 0f77e movs r5, #0x15 */
L_0800f780: ;  r1=0x000003FFu;                                                                                      /* 0f780 ldr r1, [pc, #0xf8] */
               { u32 X_=(u32)(r5), Y_=(u32)(r1); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 0f782 adds r1, r5, r1 */
               r2=*(volatile u32*)((sp+4));                                                                         /* 0f784 ldr r2, [sp, #4] */
               *(volatile u16*)(r2)=(u16)r1;                                                                        /* 0f786 strh r1, [r2] */
               r1=*(volatile u32*)((sp+8));                                                                         /* 0f788 ldr r1, [sp, #8] */
               r0=(u32)(r0^(r1)); N=(r0)>>31; Z=(r0)==0;                                                            /* 0f78a eors r0, r1 */
               r1=0x0300047Cu;                                                                                      /* 0f78c ldr r1, [pc, #0xe4] */
               *(volatile u32*)(r1)=(u32)r0;                                                                        /* 0f78e str r0, [r1] */
               r5=0x03000480u;                                                                                      /* 0f790 ldr r5, [pc, #0xe4] */
               r0=*(volatile u32*)(r5);                                                                             /* 0f792 ldr r0, [r5] */
L_0800f794: ;  r3=*(volatile u32*)(r5);                                                                             /* 0f794 ldr r3, [r5] */
               r1=0x04000100u;                                                                                      /* 0f796 ldr r1, [pc, #0xd4] */
               r1=*(volatile u16*)((r1+48));                                                                        /* 0f798 ldrh r1, [r1, #0x30] */
               r4=0x000003FFu;                                                                                      /* 0f79a ldr r4, [pc, #0xe0] */
               r4=(u32)(r4&~(r1)); N=(r4)>>31; Z=(r4)==0;                                                           /* 0f79c bics r4, r1 */
               { u32 X_=(u32)(0u), Y_=(~(u32)(r4)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 0f79e rsbs r2, r4, #0 */
               { u32 X_=(u32)(r2), Y_=(u32)(r4); unsigned long long W_=(unsigned long long)X_+Y_+(u32)C; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 0f7a0 adcs r2, r4 */
               r1=0x03000484u;                                                                                      /* 0f7a2 ldr r1, [pc, #0xdc] */
               r1=*(volatile u8*)(r1);                                                                              /* 0f7a4 ldrb r1, [r1] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0f7a6 cmp r1, #0 */
               r1=*(volatile u32*)((sp+8));                                                                         /* 0f7a8 ldr r1, [sp, #8] */
               if(!Z) goto L_0800f7ae;                                                                              /* 0f7aa bne #0x800f7ae */
L_0800f7ac: ;  r1=r2; N=(r1)>>31; Z=(r1)==0;                                                                        /* 0f7ac movs r1, r2 */
L_0800f7ae: ;  { u32 X_=(u32)(r3), Y_=(~(u32)(r0)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r5=R_; } /* 0f7ae subs r5, r3, r0 */
               { u32 X_=(u32)(r5), Y_=(~(u32)(4u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0f7b0 cmp r5, #4 */
               if(!C) goto L_0800f7b6;                                                                              /* 0f7b2 blo #0x800f7b6 */
L_0800f7b4: ;  r5=4u; N=(r5)>>31; Z=(r5)==0;                                                                        /* 0f7b4 movs r5, #4 */
L_0800f7b6: ;  { u32 X_=(u32)(r3), Y_=(~(u32)(r0)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0f7b6 cmp r3, r0 */
               if(!Z) goto L_0800f7bc;                                                                              /* 0f7b8 bne #0x800f7bc */
L_0800f7ba: ;  r5=1u; N=(r5)>>31; Z=(r5)==0;                                                                        /* 0f7ba movs r5, #1 */
L_0800f7bc: ;  *(volatile u32*)((sp+12))=(u32)r3;                                                                   /* 0f7bc str r3, [sp, #0xc] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0f7be cmp r1, #0 */
               if(!Z) goto L_0800f7d6;                                                                              /* 0f7c0 bne #0x800f7d6 */
L_0800f7c2: ;  r0=0x04000100u;                                                                                      /* 0f7c2 ldr r0, [pc, #0xa8] */
               r0=*(volatile u16*)(r0);                                                                             /* 0f7c4 ldrh r0, [r0] */
               r1=*(volatile u32*)((sp+12));                                                                        /* 0f7c6 ldr r1, [sp, #0xc] */
               { u32 X_=(u32)(r1), S_=(u32)(16u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 0f7c8 lsls r1, r1, #0x10 */
               { u32 X_=(u32)(r1), Y_=(u32)(r0); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 0f7ca adds r0, r1, r0 */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&o_rng_mix)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* o_rng_mix */ /* 0f7cc bl #0x800e840 */
               r0=*(volatile u32*)((sp+8));                                                                         /* 0f7d0 ldr r0, [sp, #8] */
               r1=0x03000484u;                                                                                      /* 0f7d2 ldr r1, [pc, #0xac] */
               *(volatile u8*)(r1)=(u8)r0;                                                                          /* 0f7d4 strb r0, [r1] */
L_0800f7d6: ;  { u32 X_=(u32)(r5), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0f7d6 cmp r5, #0 */
               if(Z) goto L_0800f814;                                                                               /* 0f7d8 beq #0x800f814 */
L_0800f7da: ;  r0=r4; N=(r0)>>31; Z=(r0)==0;                                                                        /* 0f7da movs r0, r4 */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&hook_update)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* hook_update */ /* 0f7dc bl #0x80001ac */
               { u32 X_=(u32)(r5), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0f7e0 cmp r5, #1 */
               if(Z) goto L_0800f814;                                                                               /* 0f7e2 beq #0x800f814 */
L_0800f7e4: ;  r0=r4; N=(r0)>>31; Z=(r0)==0;                                                                        /* 0f7e4 movs r0, r4 */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&hook_update)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* hook_update */ /* 0f7e6 bl #0x80001ac */
               { u32 X_=(u32)(r5), Y_=(~(u32)(2u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0f7ea cmp r5, #2 */
               if(Z) goto L_0800f814;                                                                               /* 0f7ec beq #0x800f814 */
L_0800f7ee: ;  r0=r4; N=(r0)>>31; Z=(r0)==0;                                                                        /* 0f7ee movs r0, r4 */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&hook_update)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* hook_update */ /* 0f7f0 bl #0x80001ac */
               { u32 X_=(u32)(r5), Y_=(~(u32)(3u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0f7f4 cmp r5, #3 */
               if(Z) goto L_0800f814;                                                                               /* 0f7f6 beq #0x800f814 */
L_0800f7f8: ;  r0=r4; N=(r0)>>31; Z=(r0)==0;                                                                        /* 0f7f8 movs r0, r4 */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&hook_update)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* hook_update */ /* 0f7fa bl #0x80001ac */
               { u32 X_=(u32)(r5), Y_=(~(u32)(4u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0f7fe cmp r5, #4 */
               if(Z) goto L_0800f814;                                                                               /* 0f800 beq #0x800f814 */
L_0800f802: ;  r0=r4; N=(r0)>>31; Z=(r0)==0;                                                                        /* 0f802 movs r0, r4 */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&hook_update)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* hook_update */ /* 0f804 bl #0x80001ac */
               r0=r4; N=(r0)>>31; Z=(r0)==0;                                                                        /* 0f808 movs r0, r4 */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&hook_update)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* hook_update */ /* 0f80a bl #0x80001ac */
               r0=r4; N=(r0)>>31; Z=(r0)==0;                                                                        /* 0f80e movs r0, r4 */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&hook_update)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* hook_update */ /* 0f810 bl #0x80001ac */
L_0800f814: ;  { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&hook_render)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* hook_render */ /* 0f814 bl #0x8001fb4 */
               r5=0x03000480u;                                                                                      /* 0f818 ldr r5, [pc, #0x5c] */
               r4=*(volatile u32*)(r5);                                                                             /* 0f81a ldr r4, [r5] */
L_0800f81c: ;  r0=*(volatile u32*)(r5);                                                                             /* 0f81c ldr r0, [r5] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(r4)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0f81e cmp r0, r4 */
               if(!Z) goto L_0800f826;                                                                              /* 0f820 bne #0x800f826 */
L_0800f822: ;  __asm__ volatile("svc #2" ::: "r0","r1","r2","r3","memory");                                         /* 0f822 svc #2 */
               goto L_0800f81c;                                                                                     /* 0f824 b #0x800f81c */
L_0800f826: ;  r0=0x0300047Cu;                                                                                      /* 0f826 ldr r0, [pc, #0x4c] */
               r0=*(volatile u32*)(r0);                                                                             /* 0f828 ldr r0, [r0] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0f82a cmp r0, #0 */
               if(Z) goto L_0800f832;                                                                               /* 0f82c beq #0x800f832 */
L_0800f82e: ;  r0=21u; N=(r0)>>31; Z=(r0)==0;                                                                       /* 0f82e movs r0, #0x15 */
               goto L_0800f834;                                                                                     /* 0f830 b #0x800f834 */
L_0800f832: ;  r0=5u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 0f832 movs r0, #5 */
L_0800f834: ;  r1=0x000003FFu;                                                                                      /* 0f834 ldr r1, [pc, #0x44] */
               { u32 X_=(u32)(r1), Y_=(u32)(r0); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 0f836 adds r0, r1, r0 */
               r1=*(volatile u32*)((sp+4));                                                                         /* 0f838 ldr r1, [sp, #4] */
               *(volatile u16*)(r1)=(u16)r0;                                                                        /* 0f83a strh r0, [r1] */
               r4=0u; N=(r4)>>31; Z=(r4)==0;                                                                        /* 0f83c movs r4, #0 */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&o_build_palette)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* o_build_palette */ /* 0f83e bl #0x800cc94 */
L_0800f842: ;  r1=*(volatile u16*)((r0+r4));                                                                        /* 0f842 ldrh r1, [r0, r4] */
               *(volatile u16*)((r6+r4))=(u16)r1;                                                                   /* 0f844 strh r1, [r6, r4] */
               { u32 X_=(u32)(r4), Y_=(u32)(2u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r4=R_; } /* 0f846 adds r4, r4, #2 */
               { u32 X_=(u32)(r4), Y_=(~(u32)(128u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0f848 cmp r4, #0x80 */
               if(!Z) goto L_0800f842;                                                                              /* 0f84a bne #0x800f842 */
L_0800f84c: ;  r2=0x0300047Cu;                                                                                      /* 0f84c ldr r2, [pc, #0x24] */
               r0=*(volatile u32*)(r2);                                                                             /* 0f84e ldr r0, [r2] */
               r1=1u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 0f850 movs r1, #1 */
               r1=(u32)(r1^(r0)); N=(r1)>>31; Z=(r1)==0;                                                            /* 0f852 eors r1, r0 */
               *(volatile u32*)(r2)=(u32)r1;                                                                        /* 0f854 str r1, [r2] */
               r0=*(volatile u32*)((sp+12));                                                                        /* 0f856 ldr r0, [sp, #0xc] */
               goto L_0800f794;                                                                                     /* 0f858 b #0x800f794 */
}
