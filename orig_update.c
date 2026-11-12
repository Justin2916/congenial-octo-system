// Lifted from the original 0x080001AC-0x08001F34 by tools/lift.py: exact, not yet readable.
typedef unsigned char u8; typedef signed char s8; typedef unsigned short u16; typedef short s16;
typedef unsigned int u32; typedef int s32;
extern char o_minigame_start[];
extern char o_save_write[];
extern char o_set_message[];
extern char o_sound_play[];
void o_game_update(u32 a0){
  u32 r0=0,r1=0,r2=0,r3=0,r4=0,r5=0,r6=0,r7=0,r8=0,r9=0,r10=0,r11=0,r12=0,lr=0; u32 N=0,Z=0,C=0,V=0;
  u32 frame_[160]; u32 sp=(u32)&frame_[160];
  r0=a0;
  (void)r8;(void)r9;(void)r10;(void)r11;(void)r12;(void)lr;(void)V;(void)N;(void)Z;(void)C;
L_080001ac: ;  sp-=20; *(volatile u32*)(sp+0)=r4; *(volatile u32*)(sp+4)=r5; *(volatile u32*)(sp+8)=r6; *(volatile u32*)(sp+12)=r7; *(volatile u32*)(sp+16)=lr; /* 001ac push {r4, r5, r6, r7, lr} */
               { u32 X_=(u32)(sp), Y_=(u32)(12u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; r7=R_; } /* 001ae add r7, sp, #0xc */
               { u32 X_=(u32)(sp), Y_=(~(u32)(116u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; sp=R_; } /* 001b0 sub sp, #0x74 */
               r1=0x03000014u;                                                                                      /* 001b2 ldr r1, [pc, #0x190] */
               *(volatile u16*)(r1)=(u16)r0;                                                                        /* 001b4 strh r0, [r1] */
               r1=0x03000010u;                                                                                      /* 001b6 ldr r1, [pc, #0x190] */
               r2=*(volatile u16*)(r1);                                                                             /* 001b8 ldrh r2, [r1] */
               *(volatile u16*)(r1)=(u16)r0;                                                                        /* 001ba strh r0, [r1] */
               r0=(u32)(r0&~(r2)); N=(r0)>>31; Z=(r0)==0;                                                           /* 001bc bics r0, r2 */
               r1=0x03000012u;                                                                                      /* 001be ldr r1, [pc, #0x18c] */
               *(volatile u16*)(r1)=(u16)r0;                                                                        /* 001c0 strh r0, [r1] */
               r1=0x03000018u;                                                                                      /* 001c2 ldr r1, [pc, #0x18c] */
               r0=*(volatile u32*)(r1);                                                                             /* 001c4 ldr r0, [r1] */
               { u32 X_=(u32)(r0), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 001c6 adds r0, r0, #1 */
               *(volatile u32*)(r1)=(u32)r0;                                                                        /* 001c8 str r0, [r1] */
               r2=0x03000000u;                                                                                      /* 001ca ldr r2, [pc, #0x188] */
               r0=*(volatile u32*)(r2);                                                                             /* 001cc ldr r0, [r2] */
               { u32 X_=(u32)(r0), S_=(u32)(13u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 001ce lsls r1, r0, #0xd */
               r1=(u32)(r1^(r0)); N=(r1)>>31; Z=(r1)==0;                                                            /* 001d0 eors r1, r0 */
               { u32 X_=(u32)(r1), S_=(u32)(17u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r0=R_; N=R_>>31; Z=R_==0; } /* 001d2 lsrs r0, r1, #0x11 */
               r0=(u32)(r0^(r1)); N=(r0)>>31; Z=(r0)==0;                                                            /* 001d4 eors r0, r1 */
               { u32 X_=(u32)(r0), S_=(u32)(5u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 001d6 lsls r1, r0, #5 */
               r1=(u32)(r1^(r0)); N=(r1)>>31; Z=(r1)==0;                                                            /* 001d8 eors r1, r0 */
               *(volatile u32*)(r2)=(u32)r1;                                                                        /* 001da str r1, [r2] */
               r6=0x03000288u;                                                                                      /* 001dc ldr r6, [pc, #0x178] */
               r0=*(volatile u32*)(r6);                                                                             /* 001de ldr r0, [r6] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 001e0 cmp r0, #0 */
               if(Z) goto L_08000234;                                                                               /* 001e2 beq #0x8000234 */
L_080001e4: ;  r4=0x03000290u;                                                                                      /* 001e4 ldr r4, [pc, #0x174] */
               r1=*(volatile u8*)(r4);                                                                              /* 001e6 ldrb r1, [r4] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 001e8 cmp r1, #0 */
               if(!Z) goto L_08000230;                                                                              /* 001ea bne #0x8000230 */
L_080001ec: ;  r5=0x03000292u;                                                                                      /* 001ec ldr r5, [pc, #0x170] */
               r1=*(volatile u8*)(r5);                                                                              /* 001ee ldrb r1, [r5] */
               { u32 X_=(u32)(r1), S_=(u32)(2u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 001f0 lsls r1, r1, #2 */
               { u32 X_=(u32)(r0), Y_=(u32)(r1); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 001f2 adds r2, r0, r1 */
               r3=*(volatile u8*)((r2+2));                                                                          /* 001f4 ldrb r3, [r2, #2] */
               { u32 X_=(u32)(r3), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 001f6 cmp r3, #0 */
               if(Z) goto L_0800020a;                                                                               /* 001f8 beq #0x800020a */
L_080001fa: ;  { u32 X_=(u32)(r2), Y_=(u32)(2u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r6=R_; } /* 001fa adds r6, r2, #2 */
               r1=*(volatile u16*)((r0+r1));                                                                        /* 001fc ldrh r1, [r0, r1] */
               r0=0u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 001fe movs r0, #0 */
               { u32 X_=(u32)(r1), Y_=(~(u32)(2u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00200 cmp r1, #2 */
               if(!C) goto L_0800021c;                                                                              /* 00202 blo #0x800021c */
L_08000204: ;  r2=11u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 00204 movs r2, #0xb */
               r3=2u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 00206 movs r3, #2 */
               goto L_08000222;                                                                                     /* 00208 b #0x8000222 */
L_0800020a: ;  r4=0u; N=(r4)>>31; Z=(r4)==0;                                                                        /* 0020a movs r4, #0 */
               r0=r4; N=(r0)>>31; Z=(r0)==0;                                                                        /* 0020c movs r0, r4 */
               r1=r4; N=(r1)>>31; Z=(r1)==0;                                                                        /* 0020e movs r1, r4 */
               r2=r4; N=(r2)>>31; Z=(r2)==0;                                                                        /* 00210 movs r2, r4 */
               r3=r4; N=(r3)>>31; Z=(r3)==0;                                                                        /* 00212 movs r3, r4 */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&o_sound_play)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* o_sound_play */ /* 00214 bl #0x800f63c */
               *(volatile u32*)(r6)=(u32)r4;                                                                        /* 00218 str r4, [r6] */
               goto L_08000234;                                                                                     /* 0021a b #0x8000234 */
L_0800021c: ;  r1=r0; N=(r1)>>31; Z=(r1)==0;                                                                        /* 0021c movs r1, r0 */
               r2=r0; N=(r2)>>31; Z=(r2)==0;                                                                        /* 0021e movs r2, r0 */
               r3=r0; N=(r3)>>31; Z=(r3)==0;                                                                        /* 00220 movs r3, r0 */
L_08000222: ;  { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&o_sound_play)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* o_sound_play */ /* 00222 bl #0x800f63c */
               r1=*(volatile u8*)(r6);                                                                              /* 00226 ldrb r1, [r6] */
               r0=*(volatile u8*)(r5);                                                                              /* 00228 ldrb r0, [r5] */
               { u32 X_=(u32)(r0), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 0022a adds r0, r0, #1 */
               *(volatile u8*)(r5)=(u8)r0;                                                                          /* 0022c strb r0, [r5] */
               r6=0x03000288u;                                                                                      /* 0022e ldr r6, [pc, #0x128] */
L_08000230: ;  { u32 X_=(u32)(r1), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 00230 subs r0, r1, #1 */
               *(volatile u8*)(r4)=(u8)r0;                                                                          /* 00232 strb r0, [r4] */
L_08000234: ;  r0=0x0300028Cu;                                                                                      /* 00234 ldr r0, [pc, #0x12c] */
               r0=*(volatile u32*)(r0);                                                                             /* 00236 ldr r0, [r0] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00238 cmp r0, #0 */
               if(Z) goto L_0800028e;                                                                               /* 0023a beq #0x800028e */
L_0800023c: ;  r4=0x03000290u;                                                                                      /* 0023c ldr r4, [pc, #0x11c] */
               r1=*(volatile u8*)((r4+1));                                                                          /* 0023e ldrb r1, [r4, #1] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00240 cmp r1, #0 */
               if(!Z) goto L_0800028a;                                                                              /* 00242 bne #0x800028a */
L_08000244: ;  r5=0x03000292u;                                                                                      /* 00244 ldr r5, [pc, #0x118] */
               r1=*(volatile u8*)((r5+1));                                                                          /* 00246 ldrb r1, [r5, #1] */
               { u32 X_=(u32)(r1), S_=(u32)(2u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 00248 lsls r1, r1, #2 */
               { u32 X_=(u32)(r0), Y_=(u32)(r1); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 0024a adds r2, r0, r1 */
               r3=*(volatile u8*)((r2+2));                                                                          /* 0024c ldrb r3, [r2, #2] */
               { u32 X_=(u32)(r3), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0024e cmp r3, #0 */
               if(Z) goto L_08000262;                                                                               /* 00250 beq #0x8000262 */
L_08000252: ;  { u32 X_=(u32)(r2), Y_=(u32)(2u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r6=R_; } /* 00252 adds r6, r2, #2 */
               r1=*(volatile u16*)((r0+r1));                                                                        /* 00254 ldrh r1, [r0, r1] */
               r0=1u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00256 movs r0, #1 */
               { u32 X_=(u32)(r1), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00258 cmp r1, #1 */
               if((!C||Z)) goto L_08000276;                                                                         /* 0025a bls #0x8000276 */
L_0800025c: ;  r2=11u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 0025c movs r2, #0xb */
               r3=2u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 0025e movs r3, #2 */
               goto L_0800027c;                                                                                     /* 00260 b #0x800027c */
L_08000262: ;  r0=1u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00262 movs r0, #1 */
               r4=0u; N=(r4)>>31; Z=(r4)==0;                                                                        /* 00264 movs r4, #0 */
               r1=r4; N=(r1)>>31; Z=(r1)==0;                                                                        /* 00266 movs r1, r4 */
               r2=r4; N=(r2)>>31; Z=(r2)==0;                                                                        /* 00268 movs r2, r4 */
               r3=r4; N=(r3)>>31; Z=(r3)==0;                                                                        /* 0026a movs r3, r4 */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&o_sound_play)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* o_sound_play */ /* 0026c bl #0x800f63c */
               r0=0x0300028Cu;                                                                                      /* 00270 ldr r0, [pc, #0xf0] */
               *(volatile u32*)(r0)=(u32)r4;                                                                        /* 00272 str r4, [r0] */
               goto L_0800028e;                                                                                     /* 00274 b #0x800028e */
L_08000276: ;  r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 00276 movs r1, #0 */
               r2=r1; N=(r2)>>31; Z=(r2)==0;                                                                        /* 00278 movs r2, r1 */
               r3=r1; N=(r3)>>31; Z=(r3)==0;                                                                        /* 0027a movs r3, r1 */
L_0800027c: ;  { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&o_sound_play)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* o_sound_play */ /* 0027c bl #0x800f63c */
               r1=*(volatile u8*)(r6);                                                                              /* 00280 ldrb r1, [r6] */
               r0=*(volatile u8*)((r5+1));                                                                          /* 00282 ldrb r0, [r5, #1] */
               { u32 X_=(u32)(r0), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 00284 adds r0, r0, #1 */
               *(volatile u8*)((r5+1))=(u8)r0;                                                                      /* 00286 strb r0, [r5, #1] */
               r6=0x03000288u;                                                                                      /* 00288 ldr r6, [pc, #0xcc] */
L_0800028a: ;  { u32 X_=(u32)(r1), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 0028a subs r0, r1, #1 */
               *(volatile u8*)((r4+1))=(u8)r0;                                                                      /* 0028c strb r0, [r4, #1] */
L_0800028e: ;  r5=0x0300001Cu;                                                                                      /* 0028e ldr r5, [pc, #0xd8] */
               r0=*(volatile u32*)(r5);                                                                             /* 00290 ldr r0, [r5] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00292 cmp r0, #0 */
               if(Z) goto L_0800029a;                                                                               /* 00294 beq #0x800029a */
L_08000296: ;  { u32 X_=(u32)(r0), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 00296 subs r0, r0, #1 */
               *(volatile u32*)(r5)=(u32)r0;                                                                        /* 00298 str r0, [r5] */
L_0800029a: ;  r0=0x03000020u;                                                                                      /* 0029a ldr r0, [pc, #0xd0] */
               r1=*(volatile u32*)(r0);                                                                             /* 0029c ldr r1, [r0] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 0029e subs r0, r1, #1 */
               { u32 X_=(u32)(r0), Y_=(~(u32)(4u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 002a0 cmp r0, #4 */
               if((!C||Z)) goto L_080002a6;                                                                         /* 002a2 bls #0x80002a6 */
L_080002a4: ;  goto L_0800096c;                                                                                     /* 002a4 b #0x800096c */
L_080002a6: ;  goto L_0800096c;   /* patch.py */                                                                    /* 002a6 ldr r2, [pc, #0xc8] */
L_0800096c: ;  { u32 X_=(u32)(r1), S_=(u32)(2u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r0=R_; N=R_>>31; Z=R_==0; } /* 0096c lsls r0, r1, #2 */
               r1=0x08000978u;                                                                                      /* 0096e adr r1, #8 */
               r0=*(volatile u32*)((r1+r0));                                                                        /* 00970 ldr r0, [r1, r0] */
               switch((r0&~1u)){   case 0x080009D0u: goto L_080009d0;   case 0x08000A98u: goto L_08000a98;   case 0x08000AA8u: goto L_08000aa8;   case 0x08000B5Cu: goto L_08000b5c;   case 0x08000C18u: goto L_08000c18;   case 0x08000C4Cu: goto L_08000c4c;   case 0x08000C62u: goto L_08000c62;   case 0x08000C96u: goto L_08000c96;   case 0x08001158u: goto L_08001158;   case 0x08001682u: goto L_08001682;   default: return; }   // (never: the original has no other case) /* 00972 mov pc, r0 */
L_080009d0: ;  r5=0x030002D0u;                                                                                      /* 009d0 ldr r5, [pc, #0x3ec] */
               r0=*(volatile u8*)(r5);                                                                              /* 009d2 ldrb r0, [r5] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 009d4 cmp r0, #1 */
               if(Z) goto L_080009da;                                                                               /* 009d6 beq #0x80009da */
L_080009d8: ;  goto L_08000ccc;                                                                                     /* 009d8 b #0x8000ccc */
L_080009da: ;  r0=0x03000012u;                                                                                      /* 009da ldr r0, [pc, #0x3e8] */
               r0=*(volatile u16*)(r0);                                                                             /* 009dc ldrh r0, [r0] */
               { u32 X_=(u32)(r0), S_=(u32)(31u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r0=R_; N=R_>>31; Z=R_==0; } /* 009de lsls r0, r0, #0x1f */
               if(Z) goto L_08000a74;                                                                               /* 009e0 beq #0x8000a74 */
L_080009e2: ;  r0=0u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 009e2 movs r0, #0 */
               *(volatile u8*)(r5)=(u8)r0;                                                                          /* 009e4 strb r0, [r5] */
               r4=0x03000028u;                                                                                      /* 009e6 ldr r4, [pc, #0x3e0] */
               r2=*(volatile u8*)((r4+19));                                                                         /* 009e8 ldrb r2, [r4, #0x13] */
               r1=*(volatile u8*)((r4+18));                                                                         /* 009ea ldrb r1, [r4, #0x12] */
               r3=r0; N=(r3)>>31; Z=(r3)==0;                                                                        /* 009ec movs r3, r0 */
L_080009ee: ;  *(volatile u8*)((r4+r3))=(u8)r0;                                                                     /* 009ee strb r0, [r4, r3] */
               { u32 X_=(u32)(r3), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r3=R_; } /* 009f0 adds r3, r3, #1 */
               { u32 X_=(u32)(r3), Y_=(~(u32)(76u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 009f2 cmp r3, #0x4c */
               if(!Z) goto L_080009ee;                                                                              /* 009f4 bne #0x80009ee */
L_080009f6: ;  r0=0u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 009f6 movs r0, #0 */
               *(volatile u8*)((r4+4))=(u8)r0;                                                                      /* 009f8 strb r0, [r4, #4] */
               r3=200u; N=(r3)>>31; Z=(r3)==0;                                                                      /* 009fa movs r3, #0xc8 */
               *(volatile u16*)((r4+50))=(u16)r3;                                                                   /* 009fc strh r3, [r4, #0x32] */
               r3=120u; N=(r3)>>31; Z=(r3)==0;                                                                      /* 009fe movs r3, #0x78 */
               *(volatile u16*)((r4+46))=(u16)r3;                                                                   /* 00a00 strh r3, [r4, #0x2e] */
               r6=100u; N=(r6)>>31; Z=(r6)==0;                                                                      /* 00a02 movs r6, #0x64 */
               *(volatile u16*)((r4+22))=(u16)r6;                                                                   /* 00a04 strh r6, [r4, #0x16] */
               r6=0x00000202u;                                                                                      /* 00a06 ldr r6, [pc, #0x3c4] */
               *(volatile u16*)((r4+6))=(u16)r6;                                                                    /* 00a08 strh r6, [r4, #6] */
               *(volatile u8*)((r4+19))=(u8)r2;                                                                     /* 00a0a strb r2, [r4, #0x13] */
               *(volatile u8*)((r4+18))=(u8)r1;                                                                     /* 00a0c strb r1, [r4, #0x12] */
               r1=0x03000004u;                                                                                      /* 00a0e ldr r1, [pc, #0x3c0] */
               *(volatile u32*)(r1)=(u32)r3;                                                                        /* 00a10 str r3, [r1] */
               r1=0x03000008u;                                                                                      /* 00a12 ldr r1, [pc, #0x3c0] */
               *(volatile u32*)(r1)=(u32)r3;                                                                        /* 00a14 str r3, [r1] */
               r1=0x0300000Cu;                                                                                      /* 00a16 ldr r1, [pc, #0x3c0] */
               r2=60u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 00a18 movs r2, #0x3c */
               *(volatile u32*)(r1)=(u32)r2;                                                                        /* 00a1a str r2, [r1] */
               r1=0x31504B43u;                                                                                      /* 00a1c ldr r1, [pc, #0x3bc] */
               *(volatile u32*)(r4)=(u32)r1;                                                                        /* 00a1e str r1, [r4] */
               r3=0x03000000u;                                                                                      /* 00a20 ldr r3, [pc, #0x3bc] */
               r1=*(volatile u32*)(r3);                                                                             /* 00a22 ldr r1, [r3] */
               { u32 X_=(u32)(r1), S_=(u32)(13u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r2=R_; N=R_>>31; Z=R_==0; } /* 00a24 lsls r2, r1, #0xd */
               r2=(u32)(r2^(r1)); N=(r2)>>31; Z=(r2)==0;                                                            /* 00a26 eors r2, r1 */
               { u32 X_=(u32)(r2), S_=(u32)(17u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r1=R_; N=R_>>31; Z=R_==0; } /* 00a28 lsrs r1, r2, #0x11 */
               r1=(u32)(r1^(r2)); N=(r1)>>31; Z=(r1)==0;                                                            /* 00a2a eors r1, r2 */
               { u32 X_=(u32)(r1), S_=(u32)(5u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r2=R_; N=R_>>31; Z=R_==0; } /* 00a2c lsls r2, r1, #5 */
               r2=(u32)(r2^(r1)); N=(r2)>>31; Z=(r2)==0;                                                            /* 00a2e eors r2, r1 */
               *(volatile u32*)(r3)=(u32)r2;                                                                        /* 00a30 str r2, [r3] */
               { u32 X_=(u32)(r2), S_=(u32)(16u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r1=R_; N=R_>>31; Z=R_==0; } /* 00a32 lsrs r1, r2, #0x10 */
               r2=10u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 00a34 movs r2, #0xa */
               r2=(u32)(r1*(r2)); N=(r2)>>31; Z=(r2)==0;                                                            /* 00a36 muls r2, r1, r2 */
               { u32 X_=(u32)(r2), S_=(u32)(16u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r1=R_; N=R_>>31; Z=R_==0; } /* 00a38 lsrs r1, r2, #0x10 */
               *(volatile u8*)((r4+5))=(u8)r1;                                                                      /* 00a3a strb r1, [r4, #5] */
               r1=0x00005EEDu;                                                                                      /* 00a3c ldr r1, [pc, #0x3a4] */
L_08000a3e: ;  r2=*(volatile u8*)((r4+r0));                                                                         /* 00a3e ldrb r2, [r4, r0] */
               r3=31u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 00a40 movs r3, #0x1f */
               r3=(u32)(r1*(r3)); N=(r3)>>31; Z=(r3)==0;                                                            /* 00a42 muls r3, r1, r3 */
               { u32 X_=(u32)(r3), Y_=(u32)(r2); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 00a44 adds r1, r3, r2 */
               { u32 X_=(u32)(r0), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 00a46 adds r0, r0, #1 */
               { u32 X_=(u32)(r0), Y_=(~(u32)(72u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00a48 cmp r0, #0x48 */
               if(!Z) goto L_08000a3e;                                                                              /* 00a4a bne #0x8000a3e */
L_08000a4c: ;  *(volatile u32*)((r4+72))=(u32)r1;                                                                   /* 00a4c str r1, [r4, #0x48] */
               r1=76u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 00a4e movs r1, #0x4c */
               r0=r4; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00a50 movs r0, r4 */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&o_save_write)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* o_save_write */ /* 00a52 bl #0x800f6c0 */
               r0=1u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00a56 movs r0, #1 */
               r1=0x03000020u;                                                                                      /* 00a58 ldr r1, [pc, #0x38c] */
               *(volatile u32*)(r1)=(u32)r0;                                                                        /* 00a5a str r0, [r1] */
               r0=*(volatile u8*)((r4+19));                                                                         /* 00a5c ldrb r0, [r4, #0x13] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00a5e cmp r0, #0 */
               r6=0x03000288u;                                                                                      /* 00a60 ldr r6, [pc, #0x388] */
               if(Z) goto L_08000a74;                                                                               /* 00a62 beq #0x8000a74 */
L_08000a64: ;  r0=0x03000292u;                                                                                      /* 00a64 ldr r0, [pc, #0x388] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 00a66 movs r1, #0 */
               *(volatile u8*)((r0+1))=(u8)r1;                                                                      /* 00a68 strb r1, [r0, #1] */
               r0=0x0800FB90u;                                                                                      /* 00a6a ldr r0, [pc, #0x388] */
               r2=0x0300028Cu;                                                                                      /* 00a6c ldr r2, [pc, #0x388] */
               *(volatile u32*)(r2)=(u32)r0;                                                                        /* 00a6e str r0, [r2] */
               r0=0x03000290u;                                                                                      /* 00a70 ldr r0, [pc, #0x388] */
               *(volatile u8*)((r0+1))=(u8)r1;                                                                      /* 00a72 strb r1, [r0, #1] */
L_08000a74: ;  r0=0x03000012u;                                                                                      /* 00a74 ldr r0, [pc, #0x34c] */
               r0=*(volatile u16*)(r0);                                                                             /* 00a76 ldrh r0, [r0] */
               { u32 X_=(u32)(r0), S_=(u32)(30u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r0=R_; N=R_>>31; Z=R_==0; } /* 00a78 lsls r0, r0, #0x1e */
               if(N) goto L_08000a80;                                                                               /* 00a7a bmi #0x8000a80 */
L_08000a7c: ;  goto L_08001f2c;   /* long branch */                                                                 /* 00a7c bl #0x8001f2c */
L_08000a80: ;  r0=0u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00a80 movs r0, #0 */
               *(volatile u8*)(r5)=(u8)r0;                                                                          /* 00a82 strb r0, [r5] */
               r0=0x03000028u;                                                                                      /* 00a84 ldr r0, [pc, #0x340] */
               r0=*(volatile u8*)((r0+19));                                                                         /* 00a86 ldrb r0, [r0, #0x13] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00a88 cmp r0, #0 */
               if(!Z) goto L_08000a90;                                                                              /* 00a8a bne #0x8000a90 */
L_08000a8c: ;  goto L_08001f2c;   /* long branch */                                                                 /* 00a8c bl #0x8001f2c */
L_08000a90: ;  r0=0x0800FC0Cu;                                                                                      /* 00a90 ldr r0, [pc, #0x36c] */
               *(volatile u32*)(r6)=(u32)r0;                                                                        /* 00a92 str r0, [r6] */
               goto L_080014aa;   /* long branch */                                                                 /* 00a94 bl #0x80014aa */
L_08000a98: ;  r0=0x03000012u;                                                                                      /* 00a98 ldr r0, [pc, #0x328] */
               r0=*(volatile u16*)(r0);                                                                             /* 00a9a ldrh r0, [r0] */
               r1=11u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 00a9c movs r1, #0xb */
               { u32 R_=r0&r1; N=R_>>31; Z=R_==0; }                                                                 /* 00a9e tst r0, r1 */
               if(Z) goto L_08000aa4;                                                                               /* 00aa0 beq #0x8000aa4 */
L_08000aa2: ;  goto L_08000c14;                                                                                     /* 00aa2 b #0x8000c14 */
L_08000aa4: ;  goto L_08001f2c;   /* long branch */                                                                 /* 00aa4 bl #0x8001f2c */
L_08000aa8: ;  r0=0x030002CCu;                                                                                      /* 00aa8 ldr r0, [pc, #0x358] */
               r1=*(volatile u32*)(r0);                                                                             /* 00aaa ldr r1, [r0] */
               { u32 X_=(u32)(r1), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 00aac adds r2, r1, #1 */
               *(volatile u32*)(r0)=(u32)r2;                                                                        /* 00aae str r2, [r0] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(90u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00ab0 cmp r1, #0x5a */
               if((N==V)) goto L_08000ab8;                                                                          /* 00ab2 bge #0x8000ab8 */
L_08000ab4: ;  goto L_08001f2c;   /* long branch */                                                                 /* 00ab4 bl #0x8001f2c */
L_08000ab8: ;  r0=0x03000012u;                                                                                      /* 00ab8 ldr r0, [pc, #0x308] */
               r0=*(volatile u16*)(r0);                                                                             /* 00aba ldrh r0, [r0] */
               r1=9u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 00abc movs r1, #9 */
               { u32 R_=r0&r1; N=R_>>31; Z=R_==0; }                                                                 /* 00abe tst r0, r1 */
               if(!Z) goto L_08000ac6;                                                                              /* 00ac0 bne #0x8000ac6 */
L_08000ac2: ;  goto L_08001f2c;   /* long branch */                                                                 /* 00ac2 bl #0x8001f2c */
L_08000ac6: ;  r4=0x03000028u;                                                                                      /* 00ac6 ldr r4, [pc, #0x300] */
               r2=*(volatile u8*)((r4+19));                                                                         /* 00ac8 ldrb r2, [r4, #0x13] */
               r1=*(volatile u8*)((r4+18));                                                                         /* 00aca ldrb r1, [r4, #0x12] */
               r0=0u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00acc movs r0, #0 */
               r3=r0; N=(r3)>>31; Z=(r3)==0;                                                                        /* 00ace movs r3, r0 */
L_08000ad0: ;  *(volatile u8*)((r4+r3))=(u8)r0;                                                                     /* 00ad0 strb r0, [r4, r3] */
               { u32 X_=(u32)(r3), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r3=R_; } /* 00ad2 adds r3, r3, #1 */
               { u32 X_=(u32)(r3), Y_=(~(u32)(76u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00ad4 cmp r3, #0x4c */
               if(!Z) goto L_08000ad0;                                                                              /* 00ad6 bne #0x8000ad0 */
L_08000ad8: ;  r0=0u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00ad8 movs r0, #0 */
               *(volatile u8*)((r4+4))=(u8)r0;                                                                      /* 00ada strb r0, [r4, #4] */
               r3=200u; N=(r3)>>31; Z=(r3)==0;                                                                      /* 00adc movs r3, #0xc8 */
               *(volatile u16*)((r4+50))=(u16)r3;                                                                   /* 00ade strh r3, [r4, #0x32] */
               r3=120u; N=(r3)>>31; Z=(r3)==0;                                                                      /* 00ae0 movs r3, #0x78 */
               *(volatile u16*)((r4+46))=(u16)r3;                                                                   /* 00ae2 strh r3, [r4, #0x2e] */
               r5=100u; N=(r5)>>31; Z=(r5)==0;                                                                      /* 00ae4 movs r5, #0x64 */
               *(volatile u16*)((r4+22))=(u16)r5;                                                                   /* 00ae6 strh r5, [r4, #0x16] */
               r5=0x00000202u;                                                                                      /* 00ae8 ldr r5, [pc, #0x2e0] */
               *(volatile u16*)((r4+6))=(u16)r5;                                                                    /* 00aea strh r5, [r4, #6] */
               *(volatile u8*)((r4+19))=(u8)r2;                                                                     /* 00aec strb r2, [r4, #0x13] */
               *(volatile u8*)((r4+18))=(u8)r1;                                                                     /* 00aee strb r1, [r4, #0x12] */
               r1=0x03000004u;                                                                                      /* 00af0 ldr r1, [pc, #0x2dc] */
               *(volatile u32*)(r1)=(u32)r3;                                                                        /* 00af2 str r3, [r1] */
               r1=0x03000008u;                                                                                      /* 00af4 ldr r1, [pc, #0x2dc] */
               *(volatile u32*)(r1)=(u32)r3;                                                                        /* 00af6 str r3, [r1] */
               r1=0x0300000Cu;                                                                                      /* 00af8 ldr r1, [pc, #0x2dc] */
               r2=60u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 00afa movs r2, #0x3c */
               *(volatile u32*)(r1)=(u32)r2;                                                                        /* 00afc str r2, [r1] */
               r1=0x31504B43u;                                                                                      /* 00afe ldr r1, [pc, #0x2dc] */
               *(volatile u32*)(r4)=(u32)r1;                                                                        /* 00b00 str r1, [r4] */
               r3=0x03000000u;                                                                                      /* 00b02 ldr r3, [pc, #0x2dc] */
               r1=*(volatile u32*)(r3);                                                                             /* 00b04 ldr r1, [r3] */
               { u32 X_=(u32)(r1), S_=(u32)(13u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r2=R_; N=R_>>31; Z=R_==0; } /* 00b06 lsls r2, r1, #0xd */
               r2=(u32)(r2^(r1)); N=(r2)>>31; Z=(r2)==0;                                                            /* 00b08 eors r2, r1 */
               { u32 X_=(u32)(r2), S_=(u32)(17u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r1=R_; N=R_>>31; Z=R_==0; } /* 00b0a lsrs r1, r2, #0x11 */
               r1=(u32)(r1^(r2)); N=(r1)>>31; Z=(r1)==0;                                                            /* 00b0c eors r1, r2 */
               { u32 X_=(u32)(r1), S_=(u32)(5u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r2=R_; N=R_>>31; Z=R_==0; } /* 00b0e lsls r2, r1, #5 */
               r2=(u32)(r2^(r1)); N=(r2)>>31; Z=(r2)==0;                                                            /* 00b10 eors r2, r1 */
               *(volatile u32*)(r3)=(u32)r2;                                                                        /* 00b12 str r2, [r3] */
               { u32 X_=(u32)(r2), S_=(u32)(16u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r1=R_; N=R_>>31; Z=R_==0; } /* 00b14 lsrs r1, r2, #0x10 */
               r2=10u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 00b16 movs r2, #0xa */
               r2=(u32)(r1*(r2)); N=(r2)>>31; Z=(r2)==0;                                                            /* 00b18 muls r2, r1, r2 */
               { u32 X_=(u32)(r2), S_=(u32)(16u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r1=R_; N=R_>>31; Z=R_==0; } /* 00b1a lsrs r1, r2, #0x10 */
               *(volatile u8*)((r4+5))=(u8)r1;                                                                      /* 00b1c strb r1, [r4, #5] */
               r1=0x00005EEDu;                                                                                      /* 00b1e ldr r1, [pc, #0x2c4] */
L_08000b20: ;  r2=*(volatile u8*)((r4+r0));                                                                         /* 00b20 ldrb r2, [r4, r0] */
               r3=31u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 00b22 movs r3, #0x1f */
               r3=(u32)(r1*(r3)); N=(r3)>>31; Z=(r3)==0;                                                            /* 00b24 muls r3, r1, r3 */
               { u32 X_=(u32)(r3), Y_=(u32)(r2); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 00b26 adds r1, r3, r2 */
               { u32 X_=(u32)(r0), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 00b28 adds r0, r0, #1 */
               { u32 X_=(u32)(r0), Y_=(~(u32)(72u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00b2a cmp r0, #0x48 */
               if(!Z) goto L_08000b20;                                                                              /* 00b2c bne #0x8000b20 */
L_08000b2e: ;  *(volatile u32*)((r4+72))=(u32)r1;                                                                   /* 00b2e str r1, [r4, #0x48] */
               r1=76u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 00b30 movs r1, #0x4c */
               r0=r4; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00b32 movs r0, r4 */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&o_save_write)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* o_save_write */ /* 00b34 bl #0x800f6c0 */
               r0=1u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00b38 movs r0, #1 */
               r1=0x03000020u;                                                                                      /* 00b3a ldr r1, [pc, #0x2ac] */
               *(volatile u32*)(r1)=(u32)r0;                                                                        /* 00b3c str r0, [r1] */
               r0=*(volatile u8*)((r4+19));                                                                         /* 00b3e ldrb r0, [r4, #0x13] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00b40 cmp r0, #0 */
               if(!Z) goto L_08000b48;                                                                              /* 00b42 bne #0x8000b48 */
L_08000b44: ;  goto L_08001f2c;   /* long branch */                                                                 /* 00b44 bl #0x8001f2c */
L_08000b48: ;  r0=0x03000292u;                                                                                      /* 00b48 ldr r0, [pc, #0x2a4] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 00b4a movs r1, #0 */
               *(volatile u8*)((r0+1))=(u8)r1;                                                                      /* 00b4c strb r1, [r0, #1] */
               r0=0x0800FB90u;                                                                                      /* 00b4e ldr r0, [pc, #0x2a4] */
               r2=0x0300028Cu;                                                                                      /* 00b50 ldr r2, [pc, #0x2a4] */
               *(volatile u32*)(r2)=(u32)r0;                                                                        /* 00b52 str r0, [r2] */
               r0=0x03000290u;                                                                                      /* 00b54 ldr r0, [pc, #0x2a4] */
               *(volatile u8*)((r0+1))=(u8)r1;                                                                      /* 00b56 strb r1, [r0, #1] */
               goto L_08001f2c;   /* long branch */                                                                 /* 00b58 bl #0x8001f2c */
L_08000b5c: ;  r0=0x03000012u;                                                                                      /* 00b5c ldr r0, [pc, #0x264] */
               r0=*(volatile u16*)(r0);                                                                             /* 00b5e ldrh r0, [r0] */
               { u32 X_=(u32)(r0), S_=(u32)(24u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 00b60 lsls r1, r0, #0x18 */
               { u32 X_=(u32)(r1), S_=(u32)(30u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r1=R_; N=R_>>31; Z=R_==0; } /* 00b62 lsrs r1, r1, #0x1e */
               if(Z) goto L_08000b86;                                                                               /* 00b64 beq #0x8000b86 */
L_08000b66: ;  r1=0x030002D4u;                                                                                      /* 00b66 ldr r1, [pc, #0x2a0] */
               r2=*(volatile u32*)(r1);                                                                             /* 00b68 ldr r2, [r1] */
               r3=1u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 00b6a movs r3, #1 */
               r3=(u32)(r3^(r2)); N=(r3)>>31; Z=(r3)==0;                                                            /* 00b6c eors r3, r2 */
               *(volatile u32*)(r1)=(u32)r3;                                                                        /* 00b6e str r3, [r1] */
               r1=0x03000028u;                                                                                      /* 00b70 ldr r1, [pc, #0x254] */
               r1=*(volatile u8*)((r1+19));                                                                         /* 00b72 ldrb r1, [r1, #0x13] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00b74 cmp r1, #0 */
               if(Z) goto L_08000b86;                                                                               /* 00b76 beq #0x8000b86 */
L_08000b78: ;  r1=0x03000292u;                                                                                      /* 00b78 ldr r1, [pc, #0x274] */
               r2=0u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 00b7a movs r2, #0 */
               *(volatile u8*)(r1)=(u8)r2;                                                                          /* 00b7c strb r2, [r1] */
               r1=0x0800FC1Cu;                                                                                      /* 00b7e ldr r1, [pc, #0x28c] */
               *(volatile u32*)(r6)=(u32)r1;                                                                        /* 00b80 str r1, [r6] */
               r1=0x03000290u;                                                                                      /* 00b82 ldr r1, [pc, #0x278] */
               *(volatile u8*)(r1)=(u8)r2;                                                                          /* 00b84 strb r2, [r1] */
L_08000b86: ;  { u32 X_=(u32)(r0), S_=(u32)(30u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 00b86 lsls r1, r0, #0x1e */
               if(N) goto L_08000c14;                                                                               /* 00b88 bmi #0x8000c14 */
L_08000b8a: ;  { u32 X_=(u32)(r0), S_=(u32)(31u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r0=R_; N=R_>>31; Z=R_==0; } /* 00b8a lsls r0, r0, #0x1f */
               if(!Z) goto L_08000b92;                                                                              /* 00b8c bne #0x8000b92 */
L_08000b8e: ;  goto L_08001f2c;   /* long branch */                                                                 /* 00b8e bl #0x8001f2c */
L_08000b92: ;  r0=0x03000028u;                                                                                      /* 00b92 ldr r0, [pc, #0x234] */
               r1=0x030002D4u;                                                                                      /* 00b94 ldr r1, [pc, #0x270] */
               r2=*(volatile u32*)(r1);                                                                             /* 00b96 ldr r2, [r1] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00b98 cmp r2, #0 */
               if(Z) goto L_08000b9e;                                                                               /* 00b9a beq #0x8000b9e */
L_08000b9c: ;  goto L_080010ee;                                                                                     /* 00b9c b #0x80010ee */
L_08000b9e: ;  r1=*(volatile u8*)((r0+6));                                                                          /* 00b9e ldrb r1, [r0, #6] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(4u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00ba0 cmp r1, #4 */
               if(C) goto L_08000ba6;                                                                               /* 00ba2 bhs #0x8000ba6 */
L_08000ba4: ;  goto L_080010ee;                                                                                     /* 00ba4 b #0x80010ee */
L_08000ba6: ;  r2=0x030002A4u;                                                                                      /* 00ba6 ldr r2, [pc, #0x268] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 00ba8 movs r1, #0 */
               *(volatile u8*)((r2+10))=(u8)r1;                                                                     /* 00baa strb r1, [r2, #0xa] */
               r3=33u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 00bac movs r3, #0x21 */
               *(volatile u8*)((r2+9))=(u8)r3;                                                                      /* 00bae strb r3, [r2, #9] */
               r3=76u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 00bb0 movs r3, #0x4c */
               *(volatile u8*)((r2+8))=(u8)r3;                                                                      /* 00bb2 strb r3, [r2, #8] */
               *(volatile u8*)((r2+7))=(u8)r3;                                                                      /* 00bb4 strb r3, [r2, #7] */
               r3=85u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 00bb6 movs r3, #0x55 */
               *(volatile u8*)((r2+6))=(u8)r3;                                                                      /* 00bb8 strb r3, [r2, #6] */
               r3=70u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 00bba movs r3, #0x46 */
               *(volatile u8*)((r2+5))=(u8)r3;                                                                      /* 00bbc strb r3, [r2, #5] */
               r3=32u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 00bbe movs r3, #0x20 */
               *(volatile u8*)((r2+4))=(u8)r3;                                                                      /* 00bc0 strb r3, [r2, #4] */
               r3=83u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 00bc2 movs r3, #0x53 */
               *(volatile u8*)((r2+3))=(u8)r3;                                                                      /* 00bc4 strb r3, [r2, #3] */
               r3=39u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 00bc6 movs r3, #0x27 */
               *(volatile u8*)((r2+2))=(u8)r3;                                                                      /* 00bc8 strb r3, [r2, #2] */
               r3=84u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 00bca movs r3, #0x54 */
               *(volatile u8*)((r2+1))=(u8)r3;                                                                      /* 00bcc strb r3, [r2, #1] */
               r3=73u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 00bce movs r3, #0x49 */
               *(volatile u8*)(r2)=(u8)r3;                                                                          /* 00bd0 strb r3, [r2] */
               r2=1u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 00bd2 movs r2, #1 */
               r3=0x03000020u;                                                                                      /* 00bd4 ldr r3, [pc, #0x210] */
               *(volatile u32*)(r3)=(u32)r2;                                                                        /* 00bd6 str r2, [r3] */
               r2=100u; N=(r2)>>31; Z=(r2)==0;                                                                      /* 00bd8 movs r2, #0x64 */
               *(volatile u32*)(r5)=(u32)r2;                                                                        /* 00bda str r2, [r5] */
               r2=*(volatile u8*)((r0+19));                                                                         /* 00bdc ldrb r2, [r0, #0x13] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00bde cmp r2, #0 */
               if(Z) goto L_08000bee;                                                                               /* 00be0 beq #0x8000bee */
L_08000be2: ;  r2=0x03000292u;                                                                                      /* 00be2 ldr r2, [pc, #0x20c] */
               *(volatile u8*)(r2)=(u8)r1;                                                                          /* 00be4 strb r1, [r2] */
               r2=0x0800FC0Cu;                                                                                      /* 00be6 ldr r2, [pc, #0x218] */
               *(volatile u32*)(r6)=(u32)r2;                                                                        /* 00be8 str r2, [r6] */
               r2=0x03000290u;                                                                                      /* 00bea ldr r2, [pc, #0x210] */
               *(volatile u8*)(r2)=(u8)r1;                                                                          /* 00bec strb r1, [r2] */
L_08000bee: ;  r2=*(volatile u8*)((r0+12));                                                                         /* 00bee ldrb r2, [r0, #0xc] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r3=R_; } /* 00bf0 subs r3, r2, #1 */
               { u32 X_=(u32)(r2), Y_=(~(u32)(r3)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)C; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 00bf2 sbcs r2, r3 */
               r0=*(volatile u8*)((r0+4));                                                                          /* 00bf4 ldrb r0, [r0, #4] */
               { u32 X_=(u32)(0u), Y_=(~(u32)(r0)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r3=R_; } /* 00bf6 rsbs r3, r0, #0 */
               { u32 X_=(u32)(r3), Y_=(u32)(r0); unsigned long long W_=(unsigned long long)X_+Y_+(u32)C; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r3=R_; } /* 00bf8 adcs r3, r0 */
               r3=(u32)(r3|(r2)); N=(r3)>>31; Z=(r3)==0;                                                            /* 00bfa orrs r3, r2 */
               if(Z) goto L_08000c02;                                                                               /* 00bfc beq #0x8000c02 */
L_08000bfe: ;  goto L_08001f2c;   /* long branch */                                                                 /* 00bfe bl #0x8001f2c */
L_08000c02: ;  r0=0x0300029Cu;                                                                                      /* 00c02 ldr r0, [pc, #0x210] */
               *(volatile u32*)(r0)=(u32)r1;                                                                        /* 00c04 str r1, [r0] */
               r0=0x03000298u;                                                                                      /* 00c06 ldr r0, [pc, #0x210] */
               r2=2u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 00c08 movs r2, #2 */
               *(volatile u32*)(r0)=(u32)r2;                                                                        /* 00c0a str r2, [r0] */
               r0=0x030002A0u;                                                                                      /* 00c0c ldr r0, [pc, #0x20c] */
               *(volatile u32*)(r0)=(u32)r1;                                                                        /* 00c0e str r1, [r0] */
L_08000c10: ;  r0=3u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00c10 movs r0, #3 */
               goto L_08000ee4;                                                                                     /* 00c12 b #0x8000ee4 */
L_08000c14: ;  r0=1u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00c14 movs r0, #1 */
               goto L_08000ee4;                                                                                     /* 00c16 b #0x8000ee4 */
L_08000c18: ;  r1=0x030003ACu;                                                                                      /* 00c18 ldr r1, [pc, #0x204] */
               r2=*(volatile u32*)(r1);                                                                             /* 00c1a ldr r2, [r1] */
               r0=0x03000012u;                                                                                      /* 00c1c ldr r0, [pc, #0x1a4] */
               r0=*(volatile u16*)(r0);                                                                             /* 00c1e ldrh r0, [r0] */
               r3=17u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 00c20 movs r3, #0x11 */
               { u32 R_=r0&r3; N=R_>>31; Z=R_==0; }                                                                 /* 00c22 tst r0, r3 */
               if(!Z) goto L_08000c28;                                                                              /* 00c24 bne #0x8000c28 */
L_08000c26: ;  goto L_08000eb6;                                                                                     /* 00c26 b #0x8000eb6 */
L_08000c28: ;  { u32 X_=(u32)(r2), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00c28 cmp r2, #1 */
               if((Z||N!=V)) goto L_08000c2e;                                                                       /* 00c2a ble #0x8000c2e */
L_08000c2c: ;  goto L_08000eac;                                                                                     /* 00c2c b #0x8000eac */
L_08000c2e: ;  { u32 X_=(u32)(r2), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 00c2e adds r2, r2, #1 */
               *(volatile u32*)(r1)=(u32)r2;                                                                        /* 00c30 str r2, [r1] */
               r3=0x03000028u;                                                                                      /* 00c32 ldr r3, [pc, #0x194] */
               r3=*(volatile u8*)((r3+19));                                                                         /* 00c34 ldrb r3, [r3, #0x13] */
               { u32 X_=(u32)(r3), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00c36 cmp r3, #0 */
               if(!Z) goto L_08000c3c;                                                                              /* 00c38 bne #0x8000c3c */
L_08000c3a: ;  goto L_08000eb6;                                                                                     /* 00c3a b #0x8000eb6 */
L_08000c3c: ;  r3=0x03000292u;                                                                                      /* 00c3c ldr r3, [pc, #0x1b0] */
               r4=0u; N=(r4)>>31; Z=(r4)==0;                                                                        /* 00c3e movs r4, #0 */
               *(volatile u8*)(r3)=(u8)r4;                                                                          /* 00c40 strb r4, [r3] */
               r3=0x0800FC1Cu;                                                                                      /* 00c42 ldr r3, [pc, #0x1c8] */
               *(volatile u32*)(r6)=(u32)r3;                                                                        /* 00c44 str r3, [r6] */
               r3=0x03000290u;                                                                                      /* 00c46 ldr r3, [pc, #0x1b4] */
               *(volatile u8*)(r3)=(u8)r4;                                                                          /* 00c48 strb r4, [r3] */
               goto L_08000eb6;                                                                                     /* 00c4a b #0x8000eb6 */
L_08000c4c: ;  r0=0x03000012u;                                                                                      /* 00c4c ldr r0, [pc, #0x174] */
               r0=*(volatile u16*)(r0);                                                                             /* 00c4e ldrh r0, [r0] */
               r1=11u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 00c50 movs r1, #0xb */
               { u32 R_=r0&r1; N=R_>>31; Z=R_==0; }                                                                 /* 00c52 tst r0, r1 */
               if(!Z) goto L_08000c5a;                                                                              /* 00c54 bne #0x8000c5a */
L_08000c56: ;  goto L_08001f2c;   /* long branch */                                                                 /* 00c56 bl #0x8001f2c */
L_08000c5a: ;  r0=0u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00c5a movs r0, #0 */
               *(volatile u32*)(r5)=(u32)r0;                                                                        /* 00c5c str r0, [r5] */
               r0=1u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00c5e movs r0, #1 */
               goto L_08000ee4;                                                                                     /* 00c60 b #0x8000ee4 */
L_08000c62: ;  r0=0x03000012u;                                                                                      /* 00c62 ldr r0, [pc, #0x160] */
               r0=*(volatile u16*)(r0);                                                                             /* 00c64 ldrh r0, [r0] */
               r1=10u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 00c66 movs r1, #0xa */
               { u32 R_=r0&r1; N=R_>>31; Z=R_==0; }                                                                 /* 00c68 tst r0, r1 */
               if(Z) goto L_08000d52;                                                                               /* 00c6a beq #0x8000d52 */
L_08000c6c: ;  r0=0x03000028u;                                                                                      /* 00c6c ldr r0, [pc, #0x158] */
               r1=0x31504B43u;                                                                                      /* 00c6e ldr r1, [pc, #0x16c] */
               *(volatile u32*)(r0)=(u32)r1;                                                                        /* 00c70 str r1, [r0] */
               r1=1u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 00c72 movs r1, #1 */
               r2=0x03000020u;                                                                                      /* 00c74 ldr r2, [pc, #0x170] */
               *(volatile u32*)(r2)=(u32)r1;                                                                        /* 00c76 str r1, [r2] */
               r2=0u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 00c78 movs r2, #0 */
               r1=0x00005EEDu;                                                                                      /* 00c7a ldr r1, [pc, #0x168] */
               r3=31u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 00c7c movs r3, #0x1f */
L_08000c7e: ;  r4=*(volatile u8*)((r0+r2));                                                                         /* 00c7e ldrb r4, [r0, r2] */
               r1=(u32)(r3*(r1)); N=(r1)>>31; Z=(r1)==0;                                                            /* 00c80 muls r1, r3, r1 */
               { u32 X_=(u32)(r1), Y_=(u32)(r4); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 00c82 adds r1, r1, r4 */
               { u32 X_=(u32)(r2), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 00c84 adds r2, r2, #1 */
               { u32 X_=(u32)(r2), Y_=(~(u32)(72u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00c86 cmp r2, #0x48 */
               if(!Z) goto L_08000c7e;                                                                              /* 00c88 bne #0x8000c7e */
L_08000c8a: ;  *(volatile u32*)((r0+72))=(u32)r1;                                                                   /* 00c8a str r1, [r0, #0x48] */
L_08000c8c: ;  r1=76u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 00c8c movs r1, #0x4c */
L_08000c8e: ;  { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&o_save_write)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* o_save_write */ /* 00c8e bl #0x800f6c0 */
               goto L_08001f2c;   /* long branch */                                                                 /* 00c92 bl #0x8001f2c */
L_08000c96: ;  r1=0x03000350u;                                                                                      /* 00c96 ldr r1, [pc, #0x18c] */
               r0=*(volatile u32*)(r1);                                                                             /* 00c98 ldr r0, [r1] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00c9a cmp r0, #0 */
               if(Z) goto L_08000ca2;                                                                               /* 00c9c beq #0x8000ca2 */
L_08000c9e: ;  { u32 X_=(u32)(r0), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 00c9e subs r0, r0, #1 */
               *(volatile u32*)(r1)=(u32)r0;                                                                        /* 00ca0 str r0, [r1] */
L_08000ca2: ;  r0=0x0300034Cu;                                                                                      /* 00ca2 ldr r0, [pc, #0x184] */
               r0=*(volatile u32*)(r0);                                                                             /* 00ca4 ldr r0, [r0] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(2u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00ca6 cmp r0, #2 */
               if(!Z) goto L_08000cac;                                                                              /* 00ca8 bne #0x8000cac */
L_08000caa: ;  goto L_08000e5c;                                                                                     /* 00caa b #0x8000e5c */
L_08000cac: ;  { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00cac cmp r0, #0 */
               if(Z) goto L_08000cb2;                                                                               /* 00cae beq #0x8000cb2 */
L_08000cb0: ;  goto L_08000e94;                                                                                     /* 00cb0 b #0x8000e94 */
L_08000cb2: ;  r1=0x030002CCu;                                                                                      /* 00cb2 ldr r1, [pc, #0x150] */
               r0=*(volatile u32*)(r1);                                                                             /* 00cb4 ldr r0, [r1] */
               { u32 X_=(u32)(r0), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 00cb6 adds r2, r0, #1 */
               *(volatile u32*)(r1)=(u32)r2;                                                                        /* 00cb8 str r2, [r1] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(89u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00cba cmp r0, #0x59 */
               if((N==V)) goto L_08000cc2;                                                                          /* 00cbc bge #0x8000cc2 */
L_08000cbe: ;  goto L_08001f2c;   /* long branch */                                                                 /* 00cbe bl #0x8001f2c */
L_08000cc2: ;  r0=0u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00cc2 movs r0, #0 */
               *(volatile u32*)(r1)=(u32)r0;                                                                        /* 00cc4 str r0, [r1] */
               r1=1u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 00cc6 movs r1, #1 */
               r2=0x0300034Cu;                                                                                      /* 00cc8 ldr r2, [pc, #0x15c] */
               goto L_0800117c;                                                                                     /* 00cca b #0x800117c */
L_08000ccc: ;  r4=r6; N=(r4)>>31; Z=(r4)==0;                                                                        /* 00ccc movs r4, r6 */
               r0=0x03000012u;                                                                                      /* 00cce ldr r0, [pc, #0xf4] */
               r0=*(volatile u16*)(r0);                                                                             /* 00cd0 ldrh r0, [r0] */
               r6=0x03000280u;                                                                                      /* 00cd2 ldr r6, [pc, #0x158] */
               r1=*(volatile u8*)(r6);                                                                              /* 00cd4 ldrb r1, [r6] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00cd6 cmp r1, #1 */
               if(Z) goto L_08000cdc;                                                                               /* 00cd8 beq #0x8000cdc */
L_08000cda: ;  goto L_08000eec;                                                                                     /* 00cda b #0x8000eec */
L_08000cdc: ;  { u32 X_=(u32)(r0), S_=(u32)(24u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 00cdc lsls r1, r0, #0x18 */
               { u32 X_=(u32)(r1), S_=(u32)(30u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r1=R_; N=R_>>31; Z=R_==0; } /* 00cde lsrs r1, r1, #0x1e */
               if(Z) goto L_08000d02;                                                                               /* 00ce0 beq #0x8000d02 */
L_08000ce2: ;  r1=0x03000284u;                                                                                      /* 00ce2 ldr r1, [pc, #0x14c] */
               r2=*(volatile u32*)(r1);                                                                             /* 00ce4 ldr r2, [r1] */
               r3=1u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 00ce6 movs r3, #1 */
               r3=(u32)(r3^(r2)); N=(r3)>>31; Z=(r3)==0;                                                            /* 00ce8 eors r3, r2 */
               *(volatile u32*)(r1)=(u32)r3;                                                                        /* 00cea str r3, [r1] */
               r1=0x03000028u;                                                                                      /* 00cec ldr r1, [pc, #0xd8] */
               r1=*(volatile u8*)((r1+19));                                                                         /* 00cee ldrb r1, [r1, #0x13] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00cf0 cmp r1, #0 */
               if(Z) goto L_08000d02;                                                                               /* 00cf2 beq #0x8000d02 */
L_08000cf4: ;  r1=0x03000292u;                                                                                      /* 00cf4 ldr r1, [pc, #0xf8] */
               r2=0u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 00cf6 movs r2, #0 */
               *(volatile u8*)(r1)=(u8)r2;                                                                          /* 00cf8 strb r2, [r1] */
               r1=0x0800FC1Cu;                                                                                      /* 00cfa ldr r1, [pc, #0x110] */
               *(volatile u32*)(r4)=(u32)r1;                                                                        /* 00cfc str r1, [r4] */
               r1=0x03000290u;                                                                                      /* 00cfe ldr r1, [pc, #0xfc] */
               *(volatile u8*)(r1)=(u8)r2;                                                                          /* 00d00 strb r2, [r1] */
L_08000d02: ;  r1=9u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 00d02 movs r1, #9 */
               { u32 R_=r0&r1; N=R_>>31; Z=R_==0; }                                                                 /* 00d04 tst r0, r1 */
               if(!Z) goto L_08000d0c;                                                                              /* 00d06 bne #0x8000d0c */
L_08000d08: ;  goto L_08001f2c;   /* long branch */                                                                 /* 00d08 bl #0x8001f2c */
L_08000d0c: ;  r0=0x03000284u;                                                                                      /* 00d0c ldr r0, [pc, #0x120] */
               r0=*(volatile u32*)(r0);                                                                             /* 00d0e ldr r0, [r0] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00d10 cmp r0, #0 */
               if(!Z) goto L_08000d16;                                                                              /* 00d12 bne #0x8000d16 */
L_08000d14: ;  goto L_0800147e;                                                                                     /* 00d14 b #0x800147e */
L_08000d16: ;  r0=1u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00d16 movs r0, #1 */
               *(volatile u8*)(r5)=(u8)r0;                                                                          /* 00d18 strb r0, [r5] */
               r0=0x03000028u;                                                                                      /* 00d1a ldr r0, [pc, #0xac] */
               goto L_0800149c;                                                                                     /* 00d1c b #0x800149c */
L_08000d52: ;  { u32 X_=(u32)(r0), S_=(u32)(25u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 00d52 lsls r1, r0, #0x19 */
               if(!N) goto L_08000d78;                                                                              /* 00d54 bpl #0x8000d78 */
L_08000d56: ;  r1=0x030002D4u;                                                                                      /* 00d56 ldr r1, [pc, #0xb0] */
               r2=*(volatile u32*)(r1);                                                                             /* 00d58 ldr r2, [r1] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 00d5a subs r2, r2, #1 */
               r3=3u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 00d5c movs r3, #3 */
               r3=(u32)(r3&(r2)); N=(r3)>>31; Z=(r3)==0;                                                            /* 00d5e ands r3, r2 */
               *(volatile u32*)(r1)=(u32)r3;                                                                        /* 00d60 str r3, [r1] */
               r1=0x03000028u;                                                                                      /* 00d62 ldr r1, [pc, #0x64] */
               r1=*(volatile u8*)((r1+19));                                                                         /* 00d64 ldrb r1, [r1, #0x13] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00d66 cmp r1, #0 */
               if(Z) goto L_08000d78;                                                                               /* 00d68 beq #0x8000d78 */
L_08000d6a: ;  r1=0x03000292u;                                                                                      /* 00d6a ldr r1, [pc, #0x84] */
               r2=0u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 00d6c movs r2, #0 */
               *(volatile u8*)(r1)=(u8)r2;                                                                          /* 00d6e strb r2, [r1] */
               r1=0x0800FC1Cu;                                                                                      /* 00d70 ldr r1, [pc, #0x98] */
               *(volatile u32*)(r6)=(u32)r1;                                                                        /* 00d72 str r1, [r6] */
               r1=0x03000290u;                                                                                      /* 00d74 ldr r1, [pc, #0x84] */
               *(volatile u8*)(r1)=(u8)r2;                                                                          /* 00d76 strb r2, [r1] */
L_08000d78: ;  { u32 X_=(u32)(r0), S_=(u32)(24u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 00d78 lsls r1, r0, #0x18 */
               if(!N) goto L_08000d9e;                                                                              /* 00d7a bpl #0x8000d9e */
L_08000d7c: ;  r1=0x030002D4u;                                                                                      /* 00d7c ldr r1, [pc, #0x88] */
               r2=*(volatile u32*)(r1);                                                                             /* 00d7e ldr r2, [r1] */
               { u32 X_=(u32)(r2), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 00d80 adds r2, r2, #1 */
               r3=3u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 00d82 movs r3, #3 */
               r3=(u32)(r3&(r2)); N=(r3)>>31; Z=(r3)==0;                                                            /* 00d84 ands r3, r2 */
               *(volatile u32*)(r1)=(u32)r3;                                                                        /* 00d86 str r3, [r1] */
               r1=0x03000028u;                                                                                      /* 00d88 ldr r1, [pc, #0x3c] */
               r1=*(volatile u8*)((r1+19));                                                                         /* 00d8a ldrb r1, [r1, #0x13] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00d8c cmp r1, #0 */
               if(Z) goto L_08000d9e;                                                                               /* 00d8e beq #0x8000d9e */
L_08000d90: ;  r1=0x03000292u;                                                                                      /* 00d90 ldr r1, [pc, #0x5c] */
               r2=0u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 00d92 movs r2, #0 */
               *(volatile u8*)(r1)=(u8)r2;                                                                          /* 00d94 strb r2, [r1] */
               r1=0x0800FC1Cu;                                                                                      /* 00d96 ldr r1, [pc, #0x74] */
               *(volatile u32*)(r6)=(u32)r1;                                                                        /* 00d98 str r1, [r6] */
               r1=0x03000290u;                                                                                      /* 00d9a ldr r1, [pc, #0x60] */
               *(volatile u8*)(r1)=(u8)r2;                                                                          /* 00d9c strb r2, [r1] */
L_08000d9e: ;  r1=49u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 00d9e movs r1, #0x31 */
               { u32 R_=r0&r1; N=R_>>31; Z=R_==0; }                                                                 /* 00da0 tst r0, r1 */
               if(!Z) goto L_08000da8;                                                                              /* 00da2 bne #0x8000da8 */
L_08000da4: ;  goto L_08001f2c;   /* long branch */                                                                 /* 00da4 bl #0x8001f2c */
L_08000da8: ;  r1=0x030002D4u;                                                                                      /* 00da8 ldr r1, [pc, #0x5c] */
               r1=*(volatile u32*)(r1);                                                                             /* 00daa ldr r1, [r1] */
               { u32 X_=(u32)(r1), S_=(u32)(1u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 00dac lsls r1, r1, #1 */
               r8=r8;                                                                                               /* 00dae mov r8, r8 */
               { u32 X_=(u32)(r1), Y_=(u32)(0x08000DB4u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; r1=R_; } /* 00db0 add r1, pc */
               r1=*(volatile u16*)((r1+4));                                                                         /* 00db2 ldrh r1, [r1, #4] */
               { u32 X_=(u32)(r1), S_=(u32)(1u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 00db4 lsls r1, r1, #1 */
               switch((134221242u+r1)){   case 0x08000E34u: goto L_08000e34;   case 0x080014B8u: goto L_080014b8;   case 0x080014ECu: goto L_080014ec;   case 0x080014FEu: goto L_080014fe;   default: return; }   // (never: the original has no other case) /* 00db6 add pc, r1 */
L_08000e34: ;  { u32 X_=(u32)(r0), S_=(u32)(31u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r0=R_; N=R_>>31; Z=R_==0; } /* 00e34 lsls r0, r0, #0x1f */
               if(!Z) goto L_08000e3c;                                                                              /* 00e36 bne #0x8000e3c */
L_08000e38: ;  goto L_08001f2c;   /* long branch */                                                                 /* 00e38 bl #0x8001f2c */
L_08000e3c: ;  r0=0x03000028u;                                                                                      /* 00e3c ldr r0, [pc, #0x3e4] */
               r1=0x31504B43u;                                                                                      /* 00e3e ldr r1, [pc, #0x3e8] */
               *(volatile u32*)(r0)=(u32)r1;                                                                        /* 00e40 str r1, [r0] */
               r1=1u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 00e42 movs r1, #1 */
               r2=0x03000020u;                                                                                      /* 00e44 ldr r2, [pc, #0x3e4] */
               *(volatile u32*)(r2)=(u32)r1;                                                                        /* 00e46 str r1, [r2] */
               r2=0u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 00e48 movs r2, #0 */
               r1=0x00005EEDu;                                                                                      /* 00e4a ldr r1, [pc, #0x3e4] */
               r3=31u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 00e4c movs r3, #0x1f */
L_08000e4e: ;  r4=*(volatile u8*)((r0+r2));                                                                         /* 00e4e ldrb r4, [r0, r2] */
               r1=(u32)(r3*(r1)); N=(r1)>>31; Z=(r1)==0;                                                            /* 00e50 muls r1, r3, r1 */
               { u32 X_=(u32)(r1), Y_=(u32)(r4); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 00e52 adds r1, r1, r4 */
               { u32 X_=(u32)(r2), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 00e54 adds r2, r2, #1 */
               { u32 X_=(u32)(r2), Y_=(~(u32)(72u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00e56 cmp r2, #0x48 */
               if(!Z) goto L_08000e4e;                                                                              /* 00e58 bne #0x8000e4e */
L_08000e5a: ;  goto L_08000c8a;                                                                                     /* 00e5a b #0x8000c8a */
L_08000e5c: ;  r1=0x030002CCu;                                                                                      /* 00e5c ldr r1, [pc, #0x3d4] */
               r0=*(volatile u32*)(r1);                                                                             /* 00e5e ldr r0, [r1] */
               { u32 X_=(u32)(r0), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 00e60 adds r2, r0, #1 */
               *(volatile u32*)(r1)=(u32)r2;                                                                        /* 00e62 str r2, [r1] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(30u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00e64 cmp r0, #0x1e */
               if((N==V)) goto L_08000e6c;                                                                          /* 00e66 bge #0x8000e6c */
L_08000e68: ;  goto L_08001f2c;   /* long branch */                                                                 /* 00e68 bl #0x8001f2c */
L_08000e6c: ;  { u32 X_=(u32)(r0), S_=(u32)(3u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r0=R_; N=R_>>31; Z=R_==0; } /* 00e6c lsrs r0, r0, #3 */
               { u32 X_=(u32)(r0), Y_=(~(u32)(74u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00e6e cmp r0, #0x4a */
               if((C&&!Z)) goto L_08000e80;                                                                         /* 00e70 bhi #0x8000e80 */
L_08000e72: ;  r0=0x03000012u;                                                                                      /* 00e72 ldr r0, [pc, #0x3c4] */
               r0=*(volatile u16*)(r0);                                                                             /* 00e74 ldrh r0, [r0] */
               r1=11u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 00e76 movs r1, #0xb */
               r1=(u32)(r1&(r0)); N=(r1)>>31; Z=(r1)==0;                                                            /* 00e78 ands r1, r0 */
               if(!Z) goto L_08000e80;                                                                              /* 00e7a bne #0x8000e80 */
L_08000e7c: ;  goto L_08001f2c;   /* long branch */                                                                 /* 00e7c bl #0x8001f2c */
L_08000e80: ;  r0=1u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00e80 movs r0, #1 */
               r1=0x03000020u;                                                                                      /* 00e82 ldr r1, [pc, #0x3a8] */
               *(volatile u32*)(r1)=(u32)r0;                                                                        /* 00e84 str r0, [r1] */
               r0=0x03000340u;                                                                                      /* 00e86 ldr r0, [pc, #0x3b4] */
               r0=*(volatile u32*)(r0);                                                                             /* 00e88 ldr r0, [r0] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(5u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00e8a cmp r0, #5 */
               if((Z||N!=V)) goto L_08000e90;                                                                       /* 00e8c ble #0x8000e90 */
L_08000e8e: ;  goto L_0800152a;                                                                                     /* 00e8e b #0x800152a */
L_08000e90: ;  r1=0x0801116Cu;                                                                                      /* 00e90 ldr r1, [pc, #0x3ac] */
               goto L_0800152c;                                                                                     /* 00e92 b #0x800152c */
L_08000e94: ;  r0=0x0300033Cu;                                                                                      /* 00e94 ldr r0, [pc, #0x3ac] */
               r6=*(volatile u32*)(r0);                                                                             /* 00e96 ldr r6, [r0] */
               r1=0x03000014u;                                                                                      /* 00e98 ldr r1, [pc, #0x3ac] */
               r1=*(volatile u16*)(r1);                                                                             /* 00e9a ldrh r1, [r1] */
               { u32 X_=(u32)(r1), S_=(u32)(26u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r2=R_; N=R_>>31; Z=R_==0; } /* 00e9c lsls r2, r1, #0x1a */
               if(N) goto L_08000f8a;                                                                               /* 00e9e bmi #0x8000f8a */
L_08000ea0: ;  { u32 X_=(u32)(r1), S_=(u32)(27u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 00ea0 lsls r1, r1, #0x1b */
               if(N) goto L_08000f98;                                                                               /* 00ea2 bmi #0x8000f98 */
L_08000ea4: ;  { u32 X_=(u32)(r6), Y_=(~(u32)(16u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00ea4 cmp r6, #0x10 */
               if((N==V)) goto L_08000fa8;                                                                          /* 00ea6 bge #0x8000fa8 */
L_08000ea8: ;  r6=16u; N=(r6)>>31; Z=(r6)==0;                                                                       /* 00ea8 movs r6, #0x10 */
               goto L_08000fae;                                                                                     /* 00eaa b #0x8000fae */
L_08000eac: ;  { u32 X_=(u32)(r0), S_=(u32)(31u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r3=R_; N=R_>>31; Z=R_==0; } /* 00eac lsls r3, r0, #0x1f */
               if(Z) goto L_08000eb6;                                                                               /* 00eae beq #0x8000eb6 */
L_08000eb0: ;  r3=6u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 00eb0 movs r3, #6 */
               r4=0x03000020u;                                                                                      /* 00eb2 ldr r4, [pc, #0x3d8] */
               *(volatile u32*)(r4)=(u32)r3;                                                                        /* 00eb4 str r3, [r4] */
L_08000eb6: ;  { u32 X_=(u32)(r0), S_=(u32)(26u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r3=R_; N=R_>>31; Z=R_==0; } /* 00eb6 lsls r3, r0, #0x1a */
               if(!N) goto L_08000ed8;                                                                              /* 00eb8 bpl #0x8000ed8 */
L_08000eba: ;  { u32 X_=(u32)(r2), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00eba cmp r2, #1 */
               if((N!=V)) goto L_08000ed8;                                                                          /* 00ebc blt #0x8000ed8 */
L_08000ebe: ;  { u32 X_=(u32)(r2), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 00ebe subs r2, r2, #1 */
               *(volatile u32*)(r1)=(u32)r2;                                                                        /* 00ec0 str r2, [r1] */
               r1=0x03000028u;                                                                                      /* 00ec2 ldr r1, [pc, #0x3c0] */
               r1=*(volatile u8*)((r1+19));                                                                         /* 00ec4 ldrb r1, [r1, #0x13] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00ec6 cmp r1, #0 */
               if(Z) goto L_08000ed8;                                                                               /* 00ec8 beq #0x8000ed8 */
L_08000eca: ;  r1=0x03000292u;                                                                                      /* 00eca ldr r1, [pc, #0x3d0] */
               r2=0u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 00ecc movs r2, #0 */
               *(volatile u8*)(r1)=(u8)r2;                                                                          /* 00ece strb r2, [r1] */
               r1=0x0800FC1Cu;                                                                                      /* 00ed0 ldr r1, [pc, #0x3cc] */
               *(volatile u32*)(r6)=(u32)r1;                                                                        /* 00ed2 str r1, [r6] */
               r1=0x03000290u;                                                                                      /* 00ed4 ldr r1, [pc, #0x3cc] */
               *(volatile u8*)(r1)=(u8)r2;                                                                          /* 00ed6 strb r2, [r1] */
L_08000ed8: ;  r1=10u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 00ed8 movs r1, #0xa */
               { u32 R_=r0&r1; N=R_>>31; Z=R_==0; }                                                                 /* 00eda tst r0, r1 */
               if(!Z) goto L_08000ee2;                                                                              /* 00edc bne #0x8000ee2 */
L_08000ede: ;  goto L_08001f2c;   /* long branch */                                                                 /* 00ede bl #0x8001f2c */
L_08000ee2: ;  r0=6u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00ee2 movs r0, #6 */
L_08000ee4: ;  r1=0x03000020u;                                                                                      /* 00ee4 ldr r1, [pc, #0x3a4] */
               *(volatile u32*)(r1)=(u32)r0;                                                                        /* 00ee6 str r0, [r1] */
               goto L_08001f2c;   /* long branch */                                                                 /* 00ee8 bl #0x8001f2c */
L_08000eec: ;  r1=9u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 00eec movs r1, #9 */
               { u32 R_=r0&r1; N=R_>>31; Z=R_==0; }                                                                 /* 00eee tst r0, r1 */
               if(!Z) goto L_08000ef6;                                                                              /* 00ef0 bne #0x8000ef6 */
L_08000ef2: ;  goto L_08001f2c;   /* long branch */                                                                 /* 00ef2 bl #0x8001f2c */
L_08000ef6: ;  r4=0x03000028u;                                                                                      /* 00ef6 ldr r4, [pc, #0x38c] */
               r2=*(volatile u8*)((r4+19));                                                                         /* 00ef8 ldrb r2, [r4, #0x13] */
               r1=*(volatile u8*)((r4+18));                                                                         /* 00efa ldrb r1, [r4, #0x12] */
               r0=0u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00efc movs r0, #0 */
               r3=r0; N=(r3)>>31; Z=(r3)==0;                                                                        /* 00efe movs r3, r0 */
L_08000f00: ;  *(volatile u8*)((r4+r3))=(u8)r0;                                                                     /* 00f00 strb r0, [r4, r3] */
               { u32 X_=(u32)(r3), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r3=R_; } /* 00f02 adds r3, r3, #1 */
               { u32 X_=(u32)(r3), Y_=(~(u32)(76u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00f04 cmp r3, #0x4c */
               if(!Z) goto L_08000f00;                                                                              /* 00f06 bne #0x8000f00 */
L_08000f08: ;  r0=0u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00f08 movs r0, #0 */
               *(volatile u8*)((r4+4))=(u8)r0;                                                                      /* 00f0a strb r0, [r4, #4] */
               r3=200u; N=(r3)>>31; Z=(r3)==0;                                                                      /* 00f0c movs r3, #0xc8 */
               *(volatile u16*)((r4+50))=(u16)r3;                                                                   /* 00f0e strh r3, [r4, #0x32] */
               r3=120u; N=(r3)>>31; Z=(r3)==0;                                                                      /* 00f10 movs r3, #0x78 */
               *(volatile u16*)((r4+46))=(u16)r3;                                                                   /* 00f12 strh r3, [r4, #0x2e] */
               r5=100u; N=(r5)>>31; Z=(r5)==0;                                                                      /* 00f14 movs r5, #0x64 */
               *(volatile u16*)((r4+22))=(u16)r5;                                                                   /* 00f16 strh r5, [r4, #0x16] */
               r5=0x00000202u;                                                                                      /* 00f18 ldr r5, [pc, #0x38c] */
               *(volatile u16*)((r4+6))=(u16)r5;                                                                    /* 00f1a strh r5, [r4, #6] */
               *(volatile u8*)((r4+19))=(u8)r2;                                                                     /* 00f1c strb r2, [r4, #0x13] */
               *(volatile u8*)((r4+18))=(u8)r1;                                                                     /* 00f1e strb r1, [r4, #0x12] */
               r1=0x03000004u;                                                                                      /* 00f20 ldr r1, [pc, #0x388] */
               *(volatile u32*)(r1)=(u32)r3;                                                                        /* 00f22 str r3, [r1] */
               r1=0x03000008u;                                                                                      /* 00f24 ldr r1, [pc, #0x388] */
               *(volatile u32*)(r1)=(u32)r3;                                                                        /* 00f26 str r3, [r1] */
               r1=0x0300000Cu;                                                                                      /* 00f28 ldr r1, [pc, #0x388] */
               r2=60u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 00f2a movs r2, #0x3c */
               *(volatile u32*)(r1)=(u32)r2;                                                                        /* 00f2c str r2, [r1] */
               r1=0x31504B43u;                                                                                      /* 00f2e ldr r1, [pc, #0x358] */
               *(volatile u32*)(r4)=(u32)r1;                                                                        /* 00f30 str r1, [r4] */
               r3=0x03000000u;                                                                                      /* 00f32 ldr r3, [pc, #0x384] */
               r1=*(volatile u32*)(r3);                                                                             /* 00f34 ldr r1, [r3] */
               { u32 X_=(u32)(r1), S_=(u32)(13u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r2=R_; N=R_>>31; Z=R_==0; } /* 00f36 lsls r2, r1, #0xd */
               r2=(u32)(r2^(r1)); N=(r2)>>31; Z=(r2)==0;                                                            /* 00f38 eors r2, r1 */
               { u32 X_=(u32)(r2), S_=(u32)(17u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r1=R_; N=R_>>31; Z=R_==0; } /* 00f3a lsrs r1, r2, #0x11 */
               r1=(u32)(r1^(r2)); N=(r1)>>31; Z=(r1)==0;                                                            /* 00f3c eors r1, r2 */
               { u32 X_=(u32)(r1), S_=(u32)(5u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r2=R_; N=R_>>31; Z=R_==0; } /* 00f3e lsls r2, r1, #5 */
               r2=(u32)(r2^(r1)); N=(r2)>>31; Z=(r2)==0;                                                            /* 00f40 eors r2, r1 */
               *(volatile u32*)(r3)=(u32)r2;                                                                        /* 00f42 str r2, [r3] */
               { u32 X_=(u32)(r2), S_=(u32)(16u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r1=R_; N=R_>>31; Z=R_==0; } /* 00f44 lsrs r1, r2, #0x10 */
               r2=10u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 00f46 movs r2, #0xa */
               r2=(u32)(r1*(r2)); N=(r2)>>31; Z=(r2)==0;                                                            /* 00f48 muls r2, r1, r2 */
               { u32 X_=(u32)(r2), S_=(u32)(16u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r1=R_; N=R_>>31; Z=R_==0; } /* 00f4a lsrs r1, r2, #0x10 */
               *(volatile u8*)((r4+5))=(u8)r1;                                                                      /* 00f4c strb r1, [r4, #5] */
               r1=0x00005EEDu;                                                                                      /* 00f4e ldr r1, [pc, #0x340] */
L_08000f50: ;  r2=*(volatile u8*)((r4+r0));                                                                         /* 00f50 ldrb r2, [r4, r0] */
               r3=31u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 00f52 movs r3, #0x1f */
               r3=(u32)(r1*(r3)); N=(r3)>>31; Z=(r3)==0;                                                            /* 00f54 muls r3, r1, r3 */
               { u32 X_=(u32)(r3), Y_=(u32)(r2); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 00f56 adds r1, r3, r2 */
               { u32 X_=(u32)(r0), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 00f58 adds r0, r0, #1 */
               { u32 X_=(u32)(r0), Y_=(~(u32)(72u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00f5a cmp r0, #0x48 */
               if(!Z) goto L_08000f50;                                                                              /* 00f5c bne #0x8000f50 */
L_08000f5e: ;  *(volatile u32*)((r4+72))=(u32)r1;                                                                   /* 00f5e str r1, [r4, #0x48] */
               r1=76u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 00f60 movs r1, #0x4c */
               r0=r4; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00f62 movs r0, r4 */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&o_save_write)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* o_save_write */ /* 00f64 bl #0x800f6c0 */
               r0=1u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00f68 movs r0, #1 */
               *(volatile u8*)(r6)=(u8)r0;                                                                          /* 00f6a strb r0, [r6] */
               r1=0x03000020u;                                                                                      /* 00f6c ldr r1, [pc, #0x31c] */
               *(volatile u32*)(r1)=(u32)r0;                                                                        /* 00f6e str r0, [r1] */
               r0=*(volatile u8*)((r4+19));                                                                         /* 00f70 ldrb r0, [r4, #0x13] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00f72 cmp r0, #0 */
               if(!Z) goto L_08000f7a;                                                                              /* 00f74 bne #0x8000f7a */
L_08000f76: ;  goto L_08001f2c;   /* long branch */                                                                 /* 00f76 bl #0x8001f2c */
L_08000f7a: ;  r0=0x0800FB90u;                                                                                      /* 00f7a ldr r0, [pc, #0x340] */
               r1=0x0300028Cu;                                                                                      /* 00f7c ldr r1, [pc, #0x340] */
               *(volatile u32*)(r1)=(u32)r0;                                                                        /* 00f7e str r0, [r1] */
               r0=0x03000290u;                                                                                      /* 00f80 ldr r0, [pc, #0x320] */
               { u32 X_=(u32)(r0), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 00f82 adds r0, r0, #1 */
               r1=0x03000292u;                                                                                      /* 00f84 ldr r1, [pc, #0x314] */
               { u32 X_=(u32)(r1), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 00f86 adds r1, r1, #1 */
               goto L_080014ae;                                                                                     /* 00f88 b #0x80014ae */
L_08000f8a: ;  r2=0x03000358u;                                                                                      /* 00f8a ldr r2, [pc, #0x338] */
               r3=0u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 00f8c movs r3, #0 */
               *(volatile u8*)(r2)=(u8)r3;                                                                          /* 00f8e strb r3, [r2] */
               { u32 X_=(u32)(r6), Y_=(~(u32)(3u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r6=R_; } /* 00f90 subs r6, r6, #3 */
               *(volatile u32*)(r0)=(u32)r6;                                                                        /* 00f92 str r6, [r0] */
               { u32 X_=(u32)(r1), S_=(u32)(27u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 00f94 lsls r1, r1, #0x1b */
               if(!N) goto L_08000ea4;                                                                              /* 00f96 bpl #0x8000ea4 */
L_08000f98: ;  r1=0x03000358u;                                                                                      /* 00f98 ldr r1, [pc, #0x328] */
               r2=1u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 00f9a movs r2, #1 */
               *(volatile u8*)(r1)=(u8)r2;                                                                          /* 00f9c strb r2, [r1] */
               { u32 X_=(u32)(r6), Y_=(u32)(3u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r6=R_; } /* 00f9e adds r6, r6, #3 */
               *(volatile u32*)(r0)=(u32)r6;                                                                        /* 00fa0 str r6, [r0] */
               { u32 X_=(u32)(r6), Y_=(~(u32)(16u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00fa2 cmp r6, #0x10 */
               if((N==V)) goto L_08000fa8;                                                                          /* 00fa4 bge #0x8000fa8 */
L_08000fa6: ;  goto L_08000ea8;                                                                                     /* 00fa6 b #0x8000ea8 */
L_08000fa8: ;  { u32 X_=(u32)(r6), Y_=(~(u32)(225u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00fa8 cmp r6, #0xe1 */
               if(!C) goto L_08000fb0;                                                                              /* 00faa blo #0x8000fb0 */
L_08000fac: ;  r6=224u; N=(r6)>>31; Z=(r6)==0;                                                                      /* 00fac movs r6, #0xe0 */
L_08000fae: ;  *(volatile u32*)(r0)=(u32)r6;                                                                        /* 00fae str r6, [r0] */
L_08000fb0: ;  r0=0x03000354u;                                                                                      /* 00fb0 ldr r0, [pc, #0x314] */
               r1=*(volatile u32*)(r0);                                                                             /* 00fb2 ldr r1, [r0] */
               { u32 X_=(u32)(r1), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 00fb4 adds r1, r1, #1 */
               *(volatile u32*)((sp+20))=(u32)r1;                                                                   /* 00fb6 str r1, [sp, #0x14] */
               *(volatile u32*)(r0)=(u32)r1;                                                                        /* 00fb8 str r1, [r0] */
               r0=r6; N=(r0)>>31; Z=(r0)==0;                                                                        /* 00fba movs r0, r6 */
               { u32 X_=(u32)(r0), Y_=(~(u32)(17u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 00fbc subs r0, #0x11 */
               *(volatile u32*)((sp+28))=(u32)r0;                                                                   /* 00fbe str r0, [sp, #0x1c] */
               { u32 X_=(u32)(r6), Y_=(u32)(17u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r6=R_; } /* 00fc0 adds r6, #0x11 */
               r0=0x03000028u;                                                                                      /* 00fc2 ldr r0, [pc, #0x2c0] */
               r0=*(volatile u8*)((r0+19));                                                                         /* 00fc4 ldrb r0, [r0, #0x13] */
               *(volatile u32*)((sp+24))=(u32)r0;                                                                   /* 00fc6 str r0, [sp, #0x18] */
               r0=0x03000340u;                                                                                      /* 00fc8 ldr r0, [pc, #0x2cc] */
               r0=*(volatile u32*)(r0);                                                                             /* 00fca ldr r0, [r0] */
               *(volatile u32*)((sp+32))=(u32)r0;                                                                   /* 00fcc str r0, [sp, #0x20] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 00fce movs r1, #0 */
               r5=0x030002DCu;                                                                                      /* 00fd0 ldr r5, [pc, #0x2f8] */
               goto L_08000ffc;                                                                                     /* 00fd2 b #0x8000ffc */
L_08000fd4: ;  r0=*(volatile u32*)((sp+32));                                                                        /* 00fd4 ldr r0, [sp, #0x20] */
               { u32 X_=(u32)(r0), Y_=(u32)(3u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 00fd6 adds r0, r0, #3 */
               *(volatile u32*)((sp+32))=(u32)r0;                                                                   /* 00fd8 str r0, [sp, #0x20] */
               r2=0x03000340u;                                                                                      /* 00fda ldr r2, [pc, #0x2bc] */
               *(volatile u32*)(r2)=(u32)r0;                                                                        /* 00fdc str r0, [r2] */
               r0=*(volatile u32*)((sp+24));                                                                        /* 00fde ldr r0, [sp, #0x18] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00fe0 cmp r0, #0 */
               r0=0x0800FE84u;                                                                                      /* 00fe2 ldr r0, [pc, #0x2ec] */
               if(Z) goto L_08000ff6;                                                                               /* 00fe4 beq #0x8000ff6 */
L_08000fe6: ;  r2=0u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 00fe6 movs r2, #0 */
               r3=0x03000292u;                                                                                      /* 00fe8 ldr r3, [pc, #0x2b0] */
               *(volatile u8*)(r3)=(u8)r2;                                                                          /* 00fea strb r2, [r3] */
               r2=0x03000288u;                                                                                      /* 00fec ldr r2, [pc, #0x3f8] */
               *(volatile u32*)(r2)=(u32)r0;                                                                        /* 00fee str r0, [r2] */
               r0=0x03000290u;                                                                                      /* 00ff0 ldr r0, [pc, #0x2b0] */
L_08000ff2: ;  r2=0u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 00ff2 movs r2, #0 */
               *(volatile u8*)(r0)=(u8)r2;                                                                          /* 00ff4 strb r2, [r0] */
L_08000ff6: ;  { u32 X_=(u32)(r1), Y_=(u32)(12u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 00ff6 adds r1, #0xc */
               { u32 X_=(u32)(r1), Y_=(~(u32)(96u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 00ff8 cmp r1, #0x60 */
               if(Z) goto L_08001072;                                                                               /* 00ffa beq #0x8001072 */
L_08000ffc: ;  { u32 X_=(u32)(r5), Y_=(u32)(r1); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r3=R_; } /* 00ffc adds r3, r5, r1 */
               r0=*(volatile u8*)((r3+9));                                                                          /* 00ffe ldrb r0, [r3, #9] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01000 cmp r0, #0 */
               if(Z) goto L_08000ff6;                                                                               /* 01002 beq #0x8000ff6 */
L_08001004: ;  r0=r3; N=(r0)>>31; Z=(r0)==0;                                                                        /* 01004 movs r0, r3 */
               { u32 X_=(u32)(r0), Y_=(u32)(9u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 01006 adds r0, #9 */
               r4=2u; N=(r4)>>31; Z=(r4)==0;                                                                        /* 01008 movs r4, #2 */
               r4=(u32)(s32)*(volatile s16*)((r3+r4));                                                              /* 0100a ldrsh r4, [r3, r4] */
               r2=*(volatile u32*)((r3+4));                                                                         /* 0100c ldr r2, [r3, #4] */
               { u32 X_=(u32)(r2), Y_=(u32)(r4); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 0100e adds r2, r2, r4 */
               *(volatile u32*)((r3+4))=(u32)r2;                                                                    /* 01010 str r2, [r3, #4] */
               { u32 X_=(u32)(r2), S_=(u32)(8u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=(u32)((s32)X_>>S_);} else {C=X_>>31; R_=(u32)((s32)X_>>31);} r4=R_; N=R_>>31; Z=R_==0; } /* 01012 asrs r4, r2, #8 */
               r2=r4; N=(r2)>>31; Z=(r2)==0;                                                                        /* 01014 movs r2, r4 */
               { u32 X_=(u32)(r2), Y_=(~(u32)(96u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01016 subs r2, #0x60 */
               { u32 X_=(u32)(r2), Y_=(~(u32)(16u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01018 cmp r2, #0x10 */
               if((C&&!Z)) goto L_0800104a;                                                                         /* 0101a bhi #0x800104a */
L_0800101c: ;  r4=(u32)(s32)*(volatile s16*)((r5+r1));                                                              /* 0101c ldrsh r4, [r5, r1] */
               { u32 X_=(u32)(r6), Y_=(~(u32)(r4)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0101e cmp r6, r4 */
               if((Z||N!=V)) goto L_08000ff6;                                                                       /* 01020 ble #0x8000ff6 */
L_08001022: ;  r2=*(volatile u32*)((sp+28));                                                                        /* 01022 ldr r2, [sp, #0x1c] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(r4)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01024 cmp r2, r4 */
               if((N==V)) goto L_08000ff6;                                                                          /* 01026 bge #0x8000ff6 */
L_08001028: ;  r4=0u; N=(r4)>>31; Z=(r4)==0;                                                                        /* 01028 movs r4, #0 */
               *(volatile u8*)(r0)=(u8)r4;                                                                          /* 0102a strb r4, [r0] */
               r0=*(volatile u8*)((r3+8));                                                                          /* 0102c ldrb r0, [r3, #8] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0102e cmp r0, #1 */
               if(Z) goto L_08000fd4;                                                                               /* 01030 beq #0x8000fd4 */
L_08001032: ;  { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01032 cmp r0, #0 */
               if(!Z) goto L_08001050;                                                                              /* 01034 bne #0x8001050 */
L_08001036: ;  r0=*(volatile u32*)((sp+32));                                                                        /* 01036 ldr r0, [sp, #0x20] */
               { u32 X_=(u32)(r0), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 01038 adds r0, r0, #1 */
               *(volatile u32*)((sp+32))=(u32)r0;                                                                   /* 0103a str r0, [sp, #0x20] */
               r2=0x03000340u;                                                                                      /* 0103c ldr r2, [pc, #0x258] */
               *(volatile u32*)(r2)=(u32)r0;                                                                        /* 0103e str r0, [r2] */
               r0=*(volatile u32*)((sp+24));                                                                        /* 01040 ldr r0, [sp, #0x18] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01042 cmp r0, #0 */
               r0=0x0800FE78u;                                                                                      /* 01044 ldr r0, [pc, #0x3b8] */
               if(!Z) goto L_08000fe6;                                                                              /* 01046 bne #0x8000fe6 */
L_08001048: ;  goto L_08000ff6;                                                                                     /* 01048 b #0x8000ff6 */
L_0800104a: ;  { u32 X_=(u32)(r4), Y_=(~(u32)(132u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0104a cmp r4, #0x84 */
               if((!Z&&N==V)) goto L_08000ff2;                                                                      /* 0104c bgt #0x8000ff2 */
L_0800104e: ;  goto L_08000ff6;                                                                                     /* 0104e b #0x8000ff6 */
L_08001050: ;  r0=20u; N=(r0)>>31; Z=(r0)==0;                                                                       /* 01050 movs r0, #0x14 */
               r2=0x03000350u;                                                                                      /* 01052 ldr r2, [pc, #0x3b0] */
               *(volatile u32*)(r2)=(u32)r0;                                                                        /* 01054 str r0, [r2] */
               r0=*(volatile u32*)((sp+32));                                                                        /* 01056 ldr r0, [sp, #0x20] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01058 cmp r0, #1 */
               if((!Z&&N==V)) goto L_08001060;                                                                      /* 0105a bgt #0x8001060 */
L_0800105c: ;  r0=r4; N=(r0)>>31; Z=(r0)==0;                                                                        /* 0105c movs r0, r4 */
               goto L_08001062;                                                                                     /* 0105e b #0x8001062 */
L_08001060: ;  { u32 X_=(u32)(r0), Y_=(~(u32)(2u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 01060 subs r0, r0, #2 */
L_08001062: ;  *(volatile u32*)((sp+32))=(u32)r0;                                                                   /* 01062 str r0, [sp, #0x20] */
               r2=0x03000340u;                                                                                      /* 01064 ldr r2, [pc, #0x230] */
               *(volatile u32*)(r2)=(u32)r0;                                                                        /* 01066 str r0, [r2] */
               r0=*(volatile u32*)((sp+24));                                                                        /* 01068 ldr r0, [sp, #0x18] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0106a cmp r0, #0 */
               r0=0x0800FE94u;                                                                                      /* 0106c ldr r0, [pc, #0x398] */
               if(!Z) goto L_08000fe6;                                                                              /* 0106e bne #0x8000fe6 */
L_08001070: ;  goto L_08000ff6;                                                                                     /* 01070 b #0x8000ff6 */
L_08001072: ;  r2=0x03000348u;                                                                                      /* 01072 ldr r2, [pc, #0x398] */
               r0=*(volatile u32*)(r2);                                                                             /* 01074 ldr r0, [r2] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 01076 subs r0, r0, #1 */
               *(volatile u32*)(r2)=(u32)r0;                                                                        /* 01078 str r0, [r2] */
               r4=0x03000288u;                                                                                      /* 0107a ldr r4, [pc, #0x380] */
               r6=0x03000292u;                                                                                      /* 0107c ldr r6, [pc, #0x21c] */
               if((Z||N!=V)) goto L_08001084;                                                                       /* 0107e ble #0x8001084 */
L_08001080: ;  goto L_08001e76;   /* long branch */                                                                 /* 01080 bl #0x8001e76 */
L_08001084: ;  r0=*(volatile u32*)((sp+20));                                                                        /* 01084 ldr r0, [sp, #0x14] */
               { u32 X_=(u32)(r0), S_=(u32)(6u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=(u32)((s32)X_>>S_);} else {C=X_>>31; R_=(u32)((s32)X_>>31);} r0=R_; N=R_>>31; Z=R_==0; } /* 01086 asrs r0, r0, #6 */
               *(volatile u32*)((sp+28))=(u32)r0;                                                                   /* 01088 str r0, [sp, #0x1c] */
               r0=*(volatile u8*)((r5+9));                                                                          /* 0108a ldrb r0, [r5, #9] */
               r3=0u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 0108c movs r3, #0 */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0108e cmp r0, #0 */
               *(volatile u32*)((sp+20))=(u32)r3;                                                                   /* 01090 str r3, [sp, #0x14] */
               if(!Z) goto L_08001096;                                                                              /* 01092 bne #0x8001096 */
L_08001094: ;  goto L_08001476;                                                                                     /* 01094 b #0x8001476 */
L_08001096: ;  r0=*(volatile u8*)((r5+21));                                                                         /* 01096 ldrb r0, [r5, #0x15] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01098 cmp r0, #0 */
               if(!Z) goto L_0800109e;                                                                              /* 0109a bne #0x800109e */
L_0800109c: ;  goto L_08001556;                                                                                     /* 0109c b #0x8001556 */
L_0800109e: ;  r0=33u; N=(r0)>>31; Z=(r0)==0;                                                                       /* 0109e movs r0, #0x21 */
               r0=*(volatile u8*)((r5+r0));                                                                         /* 010a0 ldrb r0, [r5, r0] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 010a2 cmp r0, #0 */
               if(!Z) goto L_080010a8;                                                                              /* 010a4 bne #0x80010a8 */
L_080010a6: ;  goto L_08001560;                                                                                     /* 010a6 b #0x8001560 */
L_080010a8: ;  r0=45u; N=(r0)>>31; Z=(r0)==0;                                                                       /* 010a8 movs r0, #0x2d */
               r0=*(volatile u8*)((r5+r0));                                                                         /* 010aa ldrb r0, [r5, r0] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 010ac cmp r0, #0 */
               if(!Z) goto L_080010b4;                                                                              /* 010ae bne #0x80010b4 */
L_080010b0: ;  goto L_08001b4c;   /* long branch */                                                                 /* 010b0 bl #0x8001b4c */
L_080010b4: ;  r0=57u; N=(r0)>>31; Z=(r0)==0;                                                                       /* 010b4 movs r0, #0x39 */
               r0=*(volatile u8*)((r5+r0));                                                                         /* 010b6 ldrb r0, [r5, r0] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 010b8 cmp r0, #0 */
               if(!Z) goto L_080010c0;                                                                              /* 010ba bne #0x80010c0 */
L_080010bc: ;  goto L_08001c0e;   /* long branch */                                                                 /* 010bc bl #0x8001c0e */
L_080010c0: ;  r0=69u; N=(r0)>>31; Z=(r0)==0;                                                                       /* 010c0 movs r0, #0x45 */
               r0=*(volatile u8*)((r5+r0));                                                                         /* 010c2 ldrb r0, [r5, r0] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 010c4 cmp r0, #0 */
               if(!Z) goto L_080010cc;                                                                              /* 010c6 bne #0x80010cc */
L_080010c8: ;  goto L_08001db4;   /* long branch */                                                                 /* 010c8 bl #0x8001db4 */
L_080010cc: ;  r0=81u; N=(r0)>>31; Z=(r0)==0;                                                                       /* 010cc movs r0, #0x51 */
               r0=*(volatile u8*)((r5+r0));                                                                         /* 010ce ldrb r0, [r5, r0] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 010d0 cmp r0, #0 */
               if(!Z) goto L_080010d8;                                                                              /* 010d2 bne #0x80010d8 */
L_080010d4: ;  goto L_08001dbc;   /* long branch */                                                                 /* 010d4 bl #0x8001dbc */
L_080010d8: ;  r0=93u; N=(r0)>>31; Z=(r0)==0;                                                                       /* 010d8 movs r0, #0x5d */
               r0=*(volatile u8*)((r5+r0));                                                                         /* 010da ldrb r0, [r5, r0] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 010dc cmp r0, #0 */
               if(!Z) goto L_080010e4;                                                                              /* 010de bne #0x80010e4 */
L_080010e0: ;  goto L_08001dd4;   /* long branch */                                                                 /* 010e0 bl #0x8001dd4 */
L_080010e4: ;  r0=0x03000000u;                                                                                      /* 010e4 ldr r0, [pc, #0x1d0] */
               r0=*(volatile u32*)(r0);                                                                             /* 010e6 ldr r0, [r0] */
               r5=*(volatile u32*)((sp+28));                                                                        /* 010e8 ldr r5, [sp, #0x1c] */
               goto L_08001e4e;   /* long branch */                                                                 /* 010ea bl #0x8001e4e */
L_080010ee: ;  r3=0x0300029Cu;                                                                                      /* 010ee ldr r3, [pc, #0x320] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 010f0 movs r1, #0 */
               *(volatile u32*)(r3)=(u32)r1;                                                                        /* 010f2 str r1, [r3] */
               r3=0x030002A0u;                                                                                      /* 010f4 ldr r3, [pc, #0x31c] */
               *(volatile u32*)(r3)=(u32)r1;                                                                        /* 010f6 str r1, [r3] */
               r3=3u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 010f8 movs r3, #3 */
               r4=0x03000020u;                                                                                      /* 010fa ldr r4, [pc, #0x190] */
               *(volatile u32*)(r4)=(u32)r3;                                                                        /* 010fc str r3, [r4] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r3=R_; } /* 010fe subs r3, r2, #1 */
               { u32 X_=(u32)(r2), Y_=(~(u32)(r3)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)C; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01100 sbcs r2, r3 */
               r3=0x03000298u;                                                                                      /* 01102 ldr r3, [pc, #0x314] */
               *(volatile u32*)(r3)=(u32)r2;                                                                        /* 01104 str r2, [r3] */
               r0=*(volatile u8*)((r0+19));                                                                         /* 01106 ldrb r0, [r0, #0x13] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01108 cmp r0, #0 */
               if(!Z) goto L_08001110;                                                                              /* 0110a bne #0x8001110 */
L_0800110c: ;  goto L_08001f2c;   /* long branch */                                                                 /* 0110c bl #0x8001f2c */
L_08001110: ;  r0=0x03000292u;                                                                                      /* 01110 ldr r0, [pc, #0x188] */
               *(volatile u8*)(r0)=(u8)r1;                                                                          /* 01112 strb r1, [r0] */
               r0=0x0800FE10u;                                                                                      /* 01114 ldr r0, [pc, #0x304] */
               goto L_08001dac;   /* long branch */                                                                 /* 01116 bl #0x8001dac */
L_08001158: ;  r0=0x03000012u;                                                                                      /* 01158 ldr r0, [pc, #0x138] */
               r0=*(volatile u16*)(r0);                                                                             /* 0115a ldrh r0, [r0] */
               { u32 X_=(u32)(r0), S_=(u32)(28u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 0115c lsls r1, r0, #0x1c */
               if(N) goto L_08001172;                                                                               /* 0115e bmi #0x8001172 */
L_08001160: ;  r1=20u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01160 movs r1, #0x14 */
               { u32 R_=r0&r1; N=R_>>31; Z=R_==0; }                                                                 /* 01162 tst r0, r1 */
               if(Z) goto L_080011e6;                                                                               /* 01164 beq #0x80011e6 */
L_08001166: ;  r1=0x030002D8u;                                                                                      /* 01166 ldr r1, [pc, #0x2bc] */
               r2=*(volatile u32*)(r1);                                                                             /* 01168 ldr r2, [r1] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(5u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0116a cmp r2, #5 */
               if((!Z&&N==V)) goto L_0800119a;                                                                      /* 0116c bgt #0x800119a */
L_0800116e: ;  { u32 X_=(u32)(r2), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 0116e adds r2, r2, #1 */
               goto L_0800119c;                                                                                     /* 01170 b #0x800119c */
L_08001172: ;  r1=0x030002D4u;                                                                                      /* 01172 ldr r1, [pc, #0x2b4] */
               r0=0u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 01174 movs r0, #0 */
               *(volatile u32*)(r1)=(u32)r0;                                                                        /* 01176 str r0, [r1] */
               r1=6u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 01178 movs r1, #6 */
L_0800117a: ;  r2=0x03000020u;                                                                                      /* 0117a ldr r2, [pc, #0x110] */
L_0800117c: ;  *(volatile u32*)(r2)=(u32)r1;                                                                        /* 0117c str r1, [r2] */
               r1=0x03000028u;                                                                                      /* 0117e ldr r1, [pc, #0x104] */
               r1=*(volatile u8*)((r1+19));                                                                         /* 01180 ldrb r1, [r1, #0x13] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01182 cmp r1, #0 */
               if(!Z) goto L_0800118a;                                                                              /* 01184 bne #0x800118a */
L_08001186: ;  goto L_08001f2c;   /* long branch */                                                                 /* 01186 bl #0x8001f2c */
L_0800118a: ;  r1=0x03000292u;                                                                                      /* 0118a ldr r1, [pc, #0x110] */
               *(volatile u8*)(r1)=(u8)r0;                                                                          /* 0118c strb r0, [r1] */
               r1=0x0800FC24u;                                                                                      /* 0118e ldr r1, [pc, #0x29c] */
               *(volatile u32*)(r6)=(u32)r1;                                                                        /* 01190 str r1, [r6] */
               r1=0x03000290u;                                                                                      /* 01192 ldr r1, [pc, #0x110] */
               *(volatile u8*)(r1)=(u8)r0;                                                                          /* 01194 strb r0, [r1] */
               goto L_08001f2c;   /* long branch */                                                                 /* 01196 bl #0x8001f2c */
L_0800119a: ;  r2=0u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 0119a movs r2, #0 */
L_0800119c: ;  *(volatile u32*)(r1)=(u32)r2;                                                                        /* 0119c str r2, [r1] */
               { u32 X_=(u32)(r2), S_=(u32)(2u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 0119e lsls r1, r2, #2 */
               r2=0x0800FC30u;                                                                                      /* 011a0 ldr r2, [pc, #0x28c] */
               r2=*(volatile u32*)((r2+r1));                                                                        /* 011a2 ldr r2, [r2, r1] */
               r1=*(volatile u8*)(r2);                                                                              /* 011a4 ldrb r1, [r2] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 011a6 cmp r1, #0 */
               if(Z) goto L_080011c6;                                                                               /* 011a8 beq #0x80011c6 */
L_080011aa: ;  { u32 X_=(u32)(r2), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 011aa adds r2, r2, #1 */
               r4=0u; N=(r4)>>31; Z=(r4)==0;                                                                        /* 011ac movs r4, #0 */
               r3=0x030002A4u;                                                                                      /* 011ae ldr r3, [pc, #0x284] */
L_080011b0: ;  *(volatile u8*)((r3+r4))=(u8)r1;                                                                     /* 011b0 strb r1, [r3, r4] */
               r1=*(volatile u8*)((r2+r4));                                                                         /* 011b2 ldrb r1, [r2, r4] */
               { u32 X_=(u32)(r4), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r5=R_; } /* 011b4 adds r5, r4, #1 */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 011b6 cmp r1, #0 */
               if(Z) goto L_080011c2;                                                                               /* 011b8 beq #0x80011c2 */
L_080011ba: ;  { u32 X_=(u32)(r4), Y_=(~(u32)(37u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 011ba cmp r4, #0x25 */
               sp-=4; *(volatile u32*)(sp+0)=r5;                                                                    /* 011bc push {r5} */
               r4=*(volatile u32*)(sp+0); sp+=4;                                                                    /* 011be pop {r4} */
               if(!C) goto L_080011b0;                                                                              /* 011c0 blo #0x80011b0 */
L_080011c2: ;  { u32 X_=(u32)(r3), Y_=(u32)(r5); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 011c2 adds r2, r3, r5 */
               goto L_080011c8;                                                                                     /* 011c4 b #0x80011c8 */
L_080011c6: ;  r2=0x030002A4u;                                                                                      /* 011c6 ldr r2, [pc, #0x26c] */
L_080011c8: ;  r1=100u; N=(r1)>>31; Z=(r1)==0;                                                                      /* 011c8 movs r1, #0x64 */
               r5=0x0300001Cu;                                                                                      /* 011ca ldr r5, [pc, #0x26c] */
               *(volatile u32*)(r5)=(u32)r1;                                                                        /* 011cc str r1, [r5] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 011ce movs r1, #0 */
               *(volatile u8*)(r2)=(u8)r1;                                                                          /* 011d0 strb r1, [r2] */
               r2=0x03000028u;                                                                                      /* 011d2 ldr r2, [pc, #0xb0] */
               r2=*(volatile u8*)((r2+19));                                                                         /* 011d4 ldrb r2, [r2, #0x13] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 011d6 cmp r2, #0 */
               if(Z) goto L_080011e6;                                                                               /* 011d8 beq #0x80011e6 */
L_080011da: ;  r2=0x03000292u;                                                                                      /* 011da ldr r2, [pc, #0xc0] */
               *(volatile u8*)(r2)=(u8)r1;                                                                          /* 011dc strb r1, [r2] */
               r2=0x0800FC1Cu;                                                                                      /* 011de ldr r2, [pc, #0xc0] */
               *(volatile u32*)(r6)=(u32)r2;                                                                        /* 011e0 str r2, [r6] */
               r2=0x03000290u;                                                                                      /* 011e2 ldr r2, [pc, #0xc0] */
               *(volatile u8*)(r2)=(u8)r1;                                                                          /* 011e4 strb r1, [r2] */
L_080011e6: ;  { u32 X_=(u32)(r0), S_=(u32)(26u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 011e6 lsls r1, r0, #0x1a */
               if(!N) goto L_0800126c;                                                                              /* 011e8 bpl #0x800126c */
L_080011ea: ;  r1=0x030002D8u;                                                                                      /* 011ea ldr r1, [pc, #0x238] */
               r2=*(volatile u32*)(r1);                                                                             /* 011ec ldr r2, [r1] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 011ee cmp r2, #1 */
               if((N!=V)) goto L_080011f6;                                                                          /* 011f0 blt #0x80011f6 */
L_080011f2: ;  { u32 X_=(u32)(r2), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 011f2 subs r2, r2, #1 */
               goto L_080011f8;                                                                                     /* 011f4 b #0x80011f8 */
L_080011f6: ;  r2=6u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 011f6 movs r2, #6 */
L_080011f8: ;  *(volatile u32*)(r1)=(u32)r2;                                                                        /* 011f8 str r2, [r1] */
               { u32 X_=(u32)(r2), S_=(u32)(2u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 011fa lsls r1, r2, #2 */
               r2=0x0800FC30u;                                                                                      /* 011fc ldr r2, [pc, #0x230] */
               r2=*(volatile u32*)((r2+r1));                                                                        /* 011fe ldr r2, [r2, r1] */
               r1=*(volatile u8*)(r2);                                                                              /* 01200 ldrb r1, [r2] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01202 cmp r1, #0 */
               if(Z) goto L_0800124c;                                                                               /* 01204 beq #0x800124c */
L_08001206: ;  { u32 X_=(u32)(r2), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01206 adds r2, r2, #1 */
               r4=0u; N=(r4)>>31; Z=(r4)==0;                                                                        /* 01208 movs r4, #0 */
               r3=0x030002A4u;                                                                                      /* 0120a ldr r3, [pc, #0x228] */
L_0800120c: ;  *(volatile u8*)((r3+r4))=(u8)r1;                                                                     /* 0120c strb r1, [r3, r4] */
               r1=*(volatile u8*)((r2+r4));                                                                         /* 0120e ldrb r1, [r2, r4] */
               { u32 X_=(u32)(r4), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r5=R_; } /* 01210 adds r5, r4, #1 */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01212 cmp r1, #0 */
               if(Z) goto L_0800121e;                                                                               /* 01214 beq #0x800121e */
L_08001216: ;  { u32 X_=(u32)(r4), Y_=(~(u32)(37u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01216 cmp r4, #0x25 */
               sp-=4; *(volatile u32*)(sp+0)=r5;                                                                    /* 01218 push {r5} */
               r4=*(volatile u32*)(sp+0); sp+=4;                                                                    /* 0121a pop {r4} */
               if(!C) goto L_0800120c;                                                                              /* 0121c blo #0x800120c */
L_0800121e: ;  { u32 X_=(u32)(r3), Y_=(u32)(r5); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 0121e adds r2, r3, r5 */
               goto L_0800124e;                                                                                     /* 01220 b #0x800124e */
L_0800124c: ;  r2=0x030002A4u;                                                                                      /* 0124c ldr r2, [pc, #0x1e4] */
L_0800124e: ;  r1=100u; N=(r1)>>31; Z=(r1)==0;                                                                      /* 0124e movs r1, #0x64 */
               r5=0x0300001Cu;                                                                                      /* 01250 ldr r5, [pc, #0x1e4] */
               *(volatile u32*)(r5)=(u32)r1;                                                                        /* 01252 str r1, [r5] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 01254 movs r1, #0 */
               *(volatile u8*)(r2)=(u8)r1;                                                                          /* 01256 strb r1, [r2] */
               r2=0x03000028u;                                                                                      /* 01258 ldr r2, [pc, #0x28] */
               r2=*(volatile u8*)((r2+19));                                                                         /* 0125a ldrb r2, [r2, #0x13] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0125c cmp r2, #0 */
               if(Z) goto L_0800126c;                                                                               /* 0125e beq #0x800126c */
L_08001260: ;  r2=0x03000292u;                                                                                      /* 01260 ldr r2, [pc, #0x38] */
               *(volatile u8*)(r2)=(u8)r1;                                                                          /* 01262 strb r1, [r2] */
               r2=0x0800FC1Cu;                                                                                      /* 01264 ldr r2, [pc, #0x38] */
               *(volatile u32*)(r6)=(u32)r2;                                                                        /* 01266 str r2, [r6] */
               r2=0x03000290u;                                                                                      /* 01268 ldr r2, [pc, #0x38] */
               *(volatile u8*)(r2)=(u8)r1;                                                                          /* 0126a strb r1, [r2] */
L_0800126c: ;  { u32 X_=(u32)(r0), S_=(u32)(24u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 0126c lsls r1, r0, #0x18 */
               { u32 X_=(u32)(r1), S_=(u32)(30u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r1=R_; N=R_>>31; Z=R_==0; } /* 0126e lsrs r1, r1, #0x1e */
               if(Z) goto L_08001328;                                                                               /* 01270 beq #0x8001328 */
L_08001272: ;  r1=0x030002D8u;                                                                                      /* 01272 ldr r1, [pc, #0x1b0] */
               r2=*(volatile u32*)(r1);                                                                             /* 01274 ldr r2, [r1] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(4u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01276 cmp r2, #4 */
               if((N!=V)) goto L_080012d4;                                                                          /* 01278 blt #0x80012d4 */
L_0800127a: ;  r3=21u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 0127a movs r3, #0x15 */
               r3=~r3; N=(r3)>>31; Z=(r3)==0;                                                                       /* 0127c mvns r3, r3 */
               { u32 X_=(u32)(r3), Y_=(u32)(18u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r3=R_; } /* 0127e adds r3, #0x12 */
               goto L_080012d6;                                                                                     /* 01280 b #0x80012d6 */
L_080012d4: ;  r3=4u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 012d4 movs r3, #4 */
L_080012d6: ;  { u32 X_=(u32)(r3), Y_=(u32)(r2); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 012d6 adds r2, r3, r2 */
               { u32 X_=(u32)(r2), Y_=(~(u32)(6u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 012d8 cmp r2, #6 */
               if((N!=V)) goto L_080012de;                                                                          /* 012da blt #0x80012de */
L_080012dc: ;  r2=6u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 012dc movs r2, #6 */
L_080012de: ;  *(volatile u32*)(r1)=(u32)r2;                                                                        /* 012de str r2, [r1] */
               { u32 X_=(u32)(r2), S_=(u32)(2u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 012e0 lsls r1, r2, #2 */
               r2=0x0800FC30u;                                                                                      /* 012e2 ldr r2, [pc, #0x14c] */
               r2=*(volatile u32*)((r2+r1));                                                                        /* 012e4 ldr r2, [r2, r1] */
               r1=*(volatile u8*)(r2);                                                                              /* 012e6 ldrb r1, [r2] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 012e8 cmp r1, #0 */
               if(Z) goto L_08001308;                                                                               /* 012ea beq #0x8001308 */
L_080012ec: ;  { u32 X_=(u32)(r2), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 012ec adds r2, r2, #1 */
               r4=0u; N=(r4)>>31; Z=(r4)==0;                                                                        /* 012ee movs r4, #0 */
               r3=0x030002A4u;                                                                                      /* 012f0 ldr r3, [pc, #0x140] */
L_080012f2: ;  *(volatile u8*)((r3+r4))=(u8)r1;                                                                     /* 012f2 strb r1, [r3, r4] */
               r1=*(volatile u8*)((r2+r4));                                                                         /* 012f4 ldrb r1, [r2, r4] */
               { u32 X_=(u32)(r4), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r5=R_; } /* 012f6 adds r5, r4, #1 */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 012f8 cmp r1, #0 */
               if(Z) goto L_08001304;                                                                               /* 012fa beq #0x8001304 */
L_080012fc: ;  { u32 X_=(u32)(r4), Y_=(~(u32)(37u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 012fc cmp r4, #0x25 */
               sp-=4; *(volatile u32*)(sp+0)=r5;                                                                    /* 012fe push {r5} */
               r4=*(volatile u32*)(sp+0); sp+=4;                                                                    /* 01300 pop {r4} */
               if(!C) goto L_080012f2;                                                                              /* 01302 blo #0x80012f2 */
L_08001304: ;  { u32 X_=(u32)(r3), Y_=(u32)(r5); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01304 adds r2, r3, r5 */
               goto L_0800130a;                                                                                     /* 01306 b #0x800130a */
L_08001308: ;  r2=0x030002A4u;                                                                                      /* 01308 ldr r2, [pc, #0x128] */
L_0800130a: ;  r1=100u; N=(r1)>>31; Z=(r1)==0;                                                                      /* 0130a movs r1, #0x64 */
               r5=0x0300001Cu;                                                                                      /* 0130c ldr r5, [pc, #0x128] */
               *(volatile u32*)(r5)=(u32)r1;                                                                        /* 0130e str r1, [r5] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 01310 movs r1, #0 */
               *(volatile u8*)(r2)=(u8)r1;                                                                          /* 01312 strb r1, [r2] */
               r2=0x03000028u;                                                                                      /* 01314 ldr r2, [pc, #0x398] */
               r2=*(volatile u8*)((r2+19));                                                                         /* 01316 ldrb r2, [r2, #0x13] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01318 cmp r2, #0 */
               if(Z) goto L_08001328;                                                                               /* 0131a beq #0x8001328 */
L_0800131c: ;  r2=0x03000292u;                                                                                      /* 0131c ldr r2, [pc, #0x394] */
               *(volatile u8*)(r2)=(u8)r1;                                                                          /* 0131e strb r1, [r2] */
               r2=0x0800FC1Cu;                                                                                      /* 01320 ldr r2, [pc, #0x394] */
               *(volatile u32*)(r6)=(u32)r2;                                                                        /* 01322 str r2, [r6] */
               r2=0x03000290u;                                                                                      /* 01324 ldr r2, [pc, #0x394] */
               *(volatile u8*)(r2)=(u8)r1;                                                                          /* 01326 strb r1, [r2] */
L_08001328: ;  { u32 X_=(u32)(r0), S_=(u32)(31u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 01328 lsls r1, r0, #0x1f */
               if(!Z) goto L_0800135c;                                                                              /* 0132a bne #0x800135c */
L_0800132c: ;  { u32 X_=(u32)(r0), S_=(u32)(30u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r0=R_; N=R_>>31; Z=R_==0; } /* 0132c lsls r0, r0, #0x1e */
               if(!N) goto L_08001334;                                                                              /* 0132e bpl #0x8001334 */
L_08001330: ;  r0=0u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 01330 movs r0, #0 */
               *(volatile u32*)(r5)=(u32)r0;                                                                        /* 01332 str r0, [r5] */
L_08001334: ;  r0=0x03000028u;                                                                                      /* 01334 ldr r0, [pc, #0x3e4] */
               r1=*(volatile u8*)((r0+12));                                                                         /* 01336 ldrb r1, [r0, #0xc] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01338 subs r2, r1, #1 */
               { u32 X_=(u32)(r1), Y_=(~(u32)(r2)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)C; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 0133a sbcs r1, r2 */
               r2=*(volatile u8*)((r0+4));                                                                          /* 0133c ldrb r2, [r0, #4] */
               { u32 X_=(u32)(0u), Y_=(~(u32)(r2)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r3=R_; } /* 0133e rsbs r3, r2, #0 */
               { u32 X_=(u32)(r3), Y_=(u32)(r2); unsigned long long W_=(unsigned long long)X_+Y_+(u32)C; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r3=R_; } /* 01340 adcs r3, r2 */
               r3=(u32)(r3|(r1)); N=(r3)>>31; Z=(r3)==0;                                                            /* 01342 orrs r3, r1 */
               if(Z) goto L_0800134a;                                                                               /* 01344 beq #0x800134a */
L_08001346: ;  goto L_08001f2c;   /* long branch */                                                                 /* 01346 bl #0x8001f2c */
L_0800134a: ;  r1=21u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 0134a movs r1, #0x15 */
               r6=~r1; N=(r6)>>31; Z=(r6)==0;                                                                       /* 0134c mvns r6, r1 */
               r0=*(volatile u8*)((r0+11));                                                                         /* 0134e ldrb r0, [r0, #0xb] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(6u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01350 cmp r0, #6 */
               if((!C||Z)) goto L_08001356;                                                                         /* 01352 bls #0x8001356 */
L_08001354: ;  goto L_08001458;                                                                                     /* 01354 b #0x8001458 */
L_08001356: ;  r0=(u32)(r6*(r0)); N=(r0)>>31; Z=(r0)==0;                                                            /* 01356 muls r0, r6, r0 */
               { u32 X_=(u32)(r0), Y_=(u32)(172u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 01358 adds r0, #0xac */
               goto L_0800145a;                                                                                     /* 0135a b #0x800145a */
L_0800135c: ;  r0=0x030002D8u;                                                                                      /* 0135c ldr r0, [pc, #0xc4] */
               r3=*(volatile u32*)(r0);                                                                             /* 0135e ldr r3, [r0] */
               r2=4u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 01360 movs r2, #4 */
               r4=r3; N=(r4)>>31; Z=(r4)==0;                                                                        /* 01362 movs r4, r3 */
               r4=(u32)(r4&~(r2)); N=(r4)>>31; Z=(r4)==0;                                                           /* 01364 bics r4, r2 */
               r0=0x03000028u;                                                                                      /* 01366 ldr r0, [pc, #0x3c8] */
               r1=*(volatile u8*)((r0+4));                                                                          /* 01368 ldrb r1, [r0, #4] */
               { u32 X_=(u32)(r4), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0136a cmp r4, #1 */
               if(Z) goto L_080013d6;                                                                               /* 0136c beq #0x80013d6 */
L_0800136e: ;  { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0136e cmp r1, #0 */
               if(!Z) goto L_080013d6;                                                                              /* 01370 bne #0x80013d6 */
L_08001372: ;  r2=0x030002A4u;                                                                                      /* 01372 ldr r2, [pc, #0xc0] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 01374 movs r1, #0 */
               *(volatile u32*)((sp+32))=(u32)r1;                                                                   /* 01376 str r1, [sp, #0x20] */
               *(volatile u8*)((r2+18))=(u8)r1;                                                                     /* 01378 strb r1, [r2, #0x12] */
               r3=33u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 0137a movs r3, #0x21 */
               *(volatile u8*)((r2+17))=(u8)r3;                                                                     /* 0137c strb r3, [r2, #0x11] */
               r3=71u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 0137e movs r3, #0x47 */
               *(volatile u8*)((r2+16))=(u8)r3;                                                                     /* 01380 strb r3, [r2, #0x10] */
               *(volatile u8*)((r2+15))=(u8)r3;                                                                     /* 01382 strb r3, [r2, #0xf] */
               r3=69u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01384 movs r3, #0x45 */
               *(volatile u8*)((r2+14))=(u8)r3;                                                                     /* 01386 strb r3, [r2, #0xe] */
               r3=32u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01388 movs r3, #0x20 */
               *(volatile u8*)((r2+13))=(u8)r3;                                                                     /* 0138a strb r3, [r2, #0xd] */
               r4=78u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 0138c movs r4, #0x4e */
               *(volatile u8*)((r2+12))=(u8)r4;                                                                     /* 0138e strb r4, [r2, #0xc] */
               r4=65u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 01390 movs r4, #0x41 */
               *(volatile u8*)((r2+11))=(u8)r4;                                                                     /* 01392 strb r4, [r2, #0xb] */
               *(volatile u8*)((r2+10))=(u8)r3;                                                                     /* 01394 strb r3, [r2, #0xa] */
               r4=76u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 01396 movs r4, #0x4c */
               *(volatile u8*)((r2+9))=(u8)r4;                                                                      /* 01398 strb r4, [r2, #9] */
               *(volatile u8*)((r2+8))=(u8)r4;                                                                      /* 0139a strb r4, [r2, #8] */
               r4=73u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 0139c movs r4, #0x49 */
               *(volatile u8*)((r2+7))=(u8)r4;                                                                      /* 0139e strb r4, [r2, #7] */
               r1=r5; N=(r1)>>31; Z=(r1)==0;                                                                        /* 013a0 movs r1, r5 */
               r5=84u; N=(r5)>>31; Z=(r5)==0;                                                                       /* 013a2 movs r5, #0x54 */
               *(volatile u8*)((r2+6))=(u8)r5;                                                                      /* 013a4 strb r5, [r2, #6] */
               r0=r6; N=(r0)>>31; Z=(r0)==0;                                                                        /* 013a6 movs r0, r6 */
               r6=83u; N=(r6)>>31; Z=(r6)==0;                                                                       /* 013a8 movs r6, #0x53 */
               *(volatile u8*)((r2+5))=(u8)r6;                                                                      /* 013aa strb r6, [r2, #5] */
               *(volatile u8*)((r2+4))=(u8)r3;                                                                      /* 013ac strb r3, [r2, #4] */
               *(volatile u8*)((r2+3))=(u8)r6;                                                                      /* 013ae strb r6, [r2, #3] */
               r6=r0; N=(r6)>>31; Z=(r6)==0;                                                                        /* 013b0 movs r6, r0 */
               r3=39u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 013b2 movs r3, #0x27 */
               *(volatile u8*)((r2+2))=(u8)r3;                                                                      /* 013b4 strb r3, [r2, #2] */
               *(volatile u8*)((r2+1))=(u8)r5;                                                                      /* 013b6 strb r5, [r2, #1] */
               *(volatile u8*)(r2)=(u8)r4;                                                                          /* 013b8 strb r4, [r2] */
               r2=100u; N=(r2)>>31; Z=(r2)==0;                                                                      /* 013ba movs r2, #0x64 */
               *(volatile u32*)(r1)=(u32)r2;                                                                        /* 013bc str r2, [r1] */
               r0=0x03000028u;                                                                                      /* 013be ldr r0, [pc, #0x370] */
               r0=*(volatile u8*)((r0+19));                                                                         /* 013c0 ldrb r0, [r0, #0x13] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 013c2 cmp r0, #0 */
               if(!Z) goto L_080013ca;                                                                              /* 013c4 bne #0x80013ca */
L_080013c6: ;  goto L_08001f2c;   /* long branch */                                                                 /* 013c6 bl #0x8001f2c */
L_080013ca: ;  r0=0x03000292u;                                                                                      /* 013ca ldr r0, [pc, #0x368] */
               r1=*(volatile u32*)((sp+32));                                                                        /* 013cc ldr r1, [sp, #0x20] */
L_080013ce: ;  *(volatile u8*)(r0)=(u8)r1;                                                                          /* 013ce strb r1, [r0] */
               r0=0x0800FC0Cu;                                                                                      /* 013d0 ldr r0, [pc, #0x368] */
               goto L_08001dac;   /* long branch */                                                                 /* 013d2 bl #0x8001dac */
L_080013d6: ;  { u32 X_=(u32)(r3), Y_=(~(u32)(6u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 013d6 cmp r3, #6 */
               if((!C||Z)) goto L_080013de;                                                                         /* 013d8 bls #0x80013de */
L_080013da: ;  goto L_08001f2c;   /* long branch */                                                                 /* 013da bl #0x8001f2c */
L_080013de: ;  r4=0x080013ECu;                                                                                      /* 013de adr r4, #0xc */
               { u32 X_=(u32)(r3), S_=(u32)(1u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r3=R_; N=R_>>31; Z=R_==0; } /* 013e0 lsls r3, r3, #1 */
               r3=*(volatile u16*)((r4+r3));                                                                        /* 013e2 ldrh r3, [r4, r3] */
               { u32 X_=(u32)(r3), S_=(u32)(1u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r3=R_; N=R_>>31; Z=R_==0; } /* 013e4 lsls r3, r3, #1 */
               switch((134222826u+r3)){   case 0x0800143Cu: goto L_0800143c;   case 0x0800156Au: goto L_0800156a;   case 0x0800158Cu: goto L_0800158c;   case 0x080015A6u: goto L_080015a6;   case 0x080015BCu: goto L_080015bc;   case 0x080015D0u: goto L_080015d0;   case 0x080015E6u: goto L_080015e6;   default: return; }   // (never: the original has no other case) /* 013e6 add pc, r3 */
L_0800143c: ;  r1=*(volatile u8*)((r0+12));                                                                         /* 0143c ldrb r1, [r0, #0xc] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0143e cmp r1, #0 */
               if(Z) goto L_08001444;                                                                               /* 01440 beq #0x8001444 */
L_08001442: ;  goto L_080015ee;                                                                                     /* 01442 b #0x80015ee */
L_08001444: ;  r3=r0; N=(r3)>>31; Z=(r3)==0;                                                                        /* 01444 movs r3, r0 */
               r2=0x030002D4u;                                                                                      /* 01446 ldr r2, [pc, #0x2f8] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 01448 movs r1, #0 */
               *(volatile u32*)(r2)=(u32)r1;                                                                        /* 0144a str r1, [r2] */
               r2=2u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 0144c movs r2, #2 */
               r0=0x03000020u;                                                                                      /* 0144e ldr r0, [pc, #0x2f4] */
               *(volatile u32*)(r0)=(u32)r2;                                                                        /* 01450 str r2, [r0] */
               r0=*(volatile u8*)((r3+19));                                                                         /* 01452 ldrb r0, [r3, #0x13] */
               goto L_08001da0;   /* long branch */                                                                 /* 01454 bl #0x8001da0 */
L_08001458: ;  r0=40u; N=(r0)>>31; Z=(r0)==0;                                                                       /* 01458 movs r0, #0x28 */
L_0800145a: ;  r1=0x03000008u;                                                                                      /* 0145a ldr r1, [pc, #0x2ec] */
               r5=*(volatile u32*)(r1);                                                                             /* 0145c ldr r5, [r1] */
               { u32 X_=(u32)(r5), Y_=(~(u32)(r0)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0145e cmp r5, r0 */
               if((Z||N!=V)) goto L_08001466;                                                                       /* 01460 ble #0x8001466 */
L_08001462: ;  *(volatile u32*)(r1)=(u32)r0;                                                                        /* 01462 str r0, [r1] */
               r5=r0; N=(r5)>>31; Z=(r5)==0;                                                                        /* 01464 movs r5, r0 */
L_08001466: ;  r2=0x0300000Cu;                                                                                      /* 01466 ldr r2, [pc, #0x2e4] */
               r1=*(volatile u32*)(r2);                                                                             /* 01468 ldr r1, [r2] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0146a cmp r1, #1 */
               if((N!=V)) goto L_0800150c;                                                                          /* 0146c blt #0x800150c */
L_0800146e: ;  { u32 X_=(u32)(r1), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 0146e subs r0, r1, #1 */
               *(volatile u32*)(r2)=(u32)r0;                                                                        /* 01470 str r0, [r2] */
               goto L_08001f2c;   /* long branch */                                                                 /* 01472 bl #0x8001f2c */
L_08001476: ;  r2=r5; N=(r2)>>31; Z=(r2)==0;                                                                        /* 01476 movs r2, r5 */
               { u32 X_=(u32)(r2), Y_=(u32)(9u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01478 adds r2, #9 */
               goto L_08001dda;   /* long branch */                                                                 /* 0147a bl #0x8001dda */
L_0800147e: ;  r0=0x03000004u;                                                                                      /* 0147e ldr r0, [pc, #0x2d0] */
               r2=120u; N=(r2)>>31; Z=(r2)==0;                                                                      /* 01480 movs r2, #0x78 */
               *(volatile u32*)(r0)=(u32)r2;                                                                        /* 01482 str r2, [r0] */
               r0=0x03000008u;                                                                                      /* 01484 ldr r0, [pc, #0x2c0] */
               *(volatile u32*)(r0)=(u32)r2;                                                                        /* 01486 str r2, [r0] */
               r0=0x0300000Cu;                                                                                      /* 01488 ldr r0, [pc, #0x2c0] */
               r2=30u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 0148a movs r2, #0x1e */
               *(volatile u32*)(r0)=(u32)r2;                                                                        /* 0148c str r2, [r0] */
               r0=0x03000028u;                                                                                      /* 0148e ldr r0, [pc, #0x2a0] */
               r2=*(volatile u8*)((r0+4));                                                                          /* 01490 ldrb r2, [r0, #4] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(8u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01492 cmp r2, #8 */
               if(Z) goto L_08001498;                                                                               /* 01494 beq #0x8001498 */
L_08001496: ;  r1=1u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 01496 movs r1, #1 */
L_08001498: ;  r2=0x03000020u;                                                                                      /* 01498 ldr r2, [pc, #0x2a8] */
               *(volatile u32*)(r2)=(u32)r1;                                                                        /* 0149a str r1, [r2] */
L_0800149c: ;  r0=*(volatile u8*)((r0+19));                                                                         /* 0149c ldrb r0, [r0, #0x13] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0149e cmp r0, #0 */
               if(!Z) goto L_080014a6;                                                                              /* 014a0 bne #0x80014a6 */
L_080014a2: ;  goto L_08001f2c;   /* long branch */                                                                 /* 014a2 bl #0x8001f2c */
L_080014a6: ;  r0=0x0800FC24u;                                                                                      /* 014a6 ldr r0, [pc, #0x2ac] */
               *(volatile u32*)(r4)=(u32)r0;                                                                        /* 014a8 str r0, [r4] */
L_080014aa: ;  r0=0x03000290u;                                                                                      /* 014aa ldr r0, [pc, #0x28c] */
               r1=0x03000292u;                                                                                      /* 014ac ldr r1, [pc, #0x284] */
L_080014ae: ;  r2=0u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 014ae movs r2, #0 */
               *(volatile u8*)(r0)=(u8)r2;                                                                          /* 014b0 strb r2, [r0] */
               *(volatile u8*)(r1)=(u8)r2;                                                                          /* 014b2 strb r2, [r1] */
               goto L_08001f2c;   /* long branch */                                                                 /* 014b4 bl #0x8001f2c */
L_080014b8: ;  r5=0x03000028u;                                                                                      /* 014b8 ldr r5, [pc, #0x274] */
               r0=*(volatile u8*)((r5+19));                                                                         /* 014ba ldrb r0, [r5, #0x13] */
               { u32 X_=(u32)(0u), Y_=(~(u32)(r0)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 014bc rsbs r1, r0, #0 */
               { u32 X_=(u32)(r1), Y_=(u32)(r0); unsigned long long W_=(unsigned long long)X_+Y_+(u32)C; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 014be adcs r1, r0 */
               *(volatile u8*)((r5+19))=(u8)r1;                                                                     /* 014c0 strb r1, [r5, #0x13] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 014c2 cmp r0, #0 */
               if(!Z) goto L_080014c8;                                                                              /* 014c4 bne #0x80014c8 */
L_080014c6: ;  goto L_080015e0;                                                                                     /* 014c6 b #0x80015e0 */
L_080014c8: ;  r4=0u; N=(r4)>>31; Z=(r4)==0;                                                                        /* 014c8 movs r4, #0 */
               *(volatile u32*)(r6)=(u32)r4;                                                                        /* 014ca str r4, [r6] */
               r0=0x0300028Cu;                                                                                      /* 014cc ldr r0, [pc, #0x288] */
               *(volatile u32*)(r0)=(u32)r4;                                                                        /* 014ce str r4, [r0] */
               r0=r4; N=(r0)>>31; Z=(r0)==0;                                                                        /* 014d0 movs r0, r4 */
               r1=r4; N=(r1)>>31; Z=(r1)==0;                                                                        /* 014d2 movs r1, r4 */
               r2=r4; N=(r2)>>31; Z=(r2)==0;                                                                        /* 014d4 movs r2, r4 */
               r3=r4; N=(r3)>>31; Z=(r3)==0;                                                                        /* 014d6 movs r3, r4 */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))0x0800F63Du)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* 0x800f63c */ /* 014d8 bl #0x800f63c */
               r0=1u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 014dc movs r0, #1 */
               r1=r4; N=(r1)>>31; Z=(r1)==0;                                                                        /* 014de movs r1, r4 */
               r2=r4; N=(r2)>>31; Z=(r2)==0;                                                                        /* 014e0 movs r2, r4 */
               r3=r4; N=(r3)>>31; Z=(r3)==0;                                                                        /* 014e2 movs r3, r4 */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))0x0800F63Du)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* 0x800f63c */ /* 014e4 bl #0x800f63c */
               r0=*(volatile u8*)((r5+19));                                                                         /* 014e8 ldrb r0, [r5, #0x13] */
               goto L_080015d8;                                                                                     /* 014ea b #0x80015d8 */
L_080014ec: ;  { u32 X_=(u32)(r0), S_=(u32)(31u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r0=R_; N=R_>>31; Z=R_==0; } /* 014ec lsls r0, r0, #0x1f */
               if(!Z) goto L_080014f4;                                                                              /* 014ee bne #0x80014f4 */
L_080014f0: ;  goto L_08001f2c;   /* long branch */                                                                 /* 014f0 bl #0x8001f2c */
L_080014f4: ;  r1=0x030003ACu;                                                                                      /* 014f4 ldr r1, [pc, #0x264] */
               r0=0u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 014f6 movs r0, #0 */
               *(volatile u32*)(r1)=(u32)r0;                                                                        /* 014f8 str r0, [r1] */
               r1=7u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 014fa movs r1, #7 */
               goto L_0800117a;                                                                                     /* 014fc b #0x800117a */
L_080014fe: ;  r0=0x03000028u;                                                                                      /* 014fe ldr r0, [pc, #0x230] */
               r1=*(volatile u8*)((r0+18));                                                                         /* 01500 ldrb r1, [r0, #0x12] */
               { u32 X_=(u32)(0u), Y_=(~(u32)(r1)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01502 rsbs r2, r1, #0 */
               { u32 X_=(u32)(r2), Y_=(u32)(r1); unsigned long long W_=(unsigned long long)X_+Y_+(u32)C; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01504 adcs r2, r1 */
               *(volatile u8*)((r0+18))=(u8)r2;                                                                     /* 01506 strb r2, [r0, #0x12] */
               r0=*(volatile u8*)((r0+19));                                                                         /* 01508 ldrb r0, [r0, #0x13] */
               goto L_080015d8;                                                                                     /* 0150a b #0x80015d8 */
L_0800150c: ;  r1=0x03000004u;                                                                                      /* 0150c ldr r1, [pc, #0x240] */
               r4=*(volatile u32*)(r1);                                                                             /* 0150e ldr r4, [r1] */
               r1=0x03000018u;                                                                                      /* 01510 ldr r1, [pc, #0x24c] */
               r1=*(volatile u8*)(r1);                                                                              /* 01512 ldrb r1, [r1] */
               { u32 X_=(u32)(r1), S_=(u32)(30u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 01514 lsls r1, r1, #0x1e */
               if(Z) goto L_0800151a;                                                                               /* 01516 beq #0x800151a */
L_08001518: ;  goto L_08001c22;                                                                                     /* 01518 b #0x8001c22 */
L_0800151a: ;  { u32 X_=(u32)(r4), Y_=(~(u32)(r5)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0151a cmp r4, r5 */
               if((N!=V)) goto L_08001520;                                                                          /* 0151c blt #0x8001520 */
L_0800151e: ;  goto L_08001b88;                                                                                     /* 0151e b #0x8001b88 */
L_08001520: ;  r6=2u; N=(r6)>>31; Z=(r6)==0;                                                                        /* 01520 movs r6, #2 */
               goto L_08001b8e;                                                                                     /* 01522 b #0x8001b8e */
L_0800152a: ;  r1=0x0801019Du;                                                                                      /* 0152a ldr r1, [pc, #0x23c] */
L_0800152c: ;  { u32 X_=(u32)(r0), Y_=(~(u32)(11u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0152c cmp r0, #0xb */
               if((Z||N!=V)) goto L_08001532;                                                                       /* 0152e ble #0x8001532 */
L_08001530: ;  r1=0x08010156u;                                                                                      /* 01530 ldr r1, [pc, #0x238] */
L_08001532: ;  r0=*(volatile u8*)(r1);                                                                              /* 01532 ldrb r0, [r1] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01534 cmp r0, #0 */
               if(!Z) goto L_0800153a;                                                                              /* 01536 bne #0x800153a */
L_08001538: ;  goto L_08001a6c;                                                                                     /* 01538 b #0x8001a6c */
L_0800153a: ;  { u32 X_=(u32)(r1), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 0153a adds r1, r1, #1 */
               r3=0u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 0153c movs r3, #0 */
               r2=0x030002A4u;                                                                                      /* 0153e ldr r2, [pc, #0x230] */
L_08001540: ;  *(volatile u8*)((r2+r3))=(u8)r0;                                                                     /* 01540 strb r0, [r2, r3] */
               r0=*(volatile u8*)((r1+r3));                                                                         /* 01542 ldrb r0, [r1, r3] */
               { u32 X_=(u32)(r3), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r4=R_; } /* 01544 adds r4, r3, #1 */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01546 cmp r0, #0 */
               if(!Z) goto L_0800154c;                                                                              /* 01548 bne #0x800154c */
L_0800154a: ;  goto L_08001a68;                                                                                     /* 0154a b #0x8001a68 */
L_0800154c: ;  { u32 X_=(u32)(r3), Y_=(~(u32)(37u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0154c cmp r3, #0x25 */
               sp-=4; *(volatile u32*)(sp+0)=r4;                                                                    /* 0154e push {r4} */
               r3=*(volatile u32*)(sp+0); sp+=4;                                                                    /* 01550 pop {r3} */
               if(!C) goto L_08001540;                                                                              /* 01552 blo #0x8001540 */
L_08001554: ;  goto L_08001a68;                                                                                     /* 01554 b #0x8001a68 */
L_08001556: ;  r2=r5; N=(r2)>>31; Z=(r2)==0;                                                                        /* 01556 movs r2, r5 */
               { u32 X_=(u32)(r2), Y_=(u32)(21u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01558 adds r2, #0x15 */
               r3=1u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 0155a movs r3, #1 */
               goto L_08001dda;   /* long branch */                                                                 /* 0155c bl #0x8001dda */
L_08001560: ;  r2=r5; N=(r2)>>31; Z=(r2)==0;                                                                        /* 01560 movs r2, r5 */
               { u32 X_=(u32)(r2), Y_=(u32)(33u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01562 adds r2, #0x21 */
               r3=2u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 01564 movs r3, #2 */
               goto L_08001dda;   /* long branch */                                                                 /* 01566 bl #0x8001dda */
L_0800156a: ;  r1=*(volatile u8*)((r0+11));                                                                         /* 0156a ldrb r1, [r0, #0xb] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0156c cmp r1, #0 */
               if(!Z) goto L_08001572;                                                                              /* 0156e bne #0x8001572 */
L_08001570: ;  goto L_08001c9e;                                                                                     /* 01570 b #0x8001c9e */
L_08001572: ;  r4=r0; N=(r4)>>31; Z=(r4)==0;                                                                        /* 01572 movs r4, r0 */
               r2=0x0300029Cu;                                                                                      /* 01574 ldr r2, [pc, #0x1fc] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 01576 movs r1, #0 */
               *(volatile u32*)(r2)=(u32)r1;                                                                        /* 01578 str r1, [r2] */
               r2=0x03000298u;                                                                                      /* 0157a ldr r2, [pc, #0x1fc] */
               r3=3u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 0157c movs r3, #3 */
               *(volatile u32*)(r2)=(u32)r3;                                                                        /* 0157e str r3, [r2] */
               r2=0x030002A0u;                                                                                      /* 01580 ldr r2, [pc, #0x1f8] */
               *(volatile u32*)(r2)=(u32)r1;                                                                        /* 01582 str r1, [r2] */
               r0=0x03000020u;                                                                                      /* 01584 ldr r0, [pc, #0x1bc] */
               *(volatile u32*)(r0)=(u32)r3;                                                                        /* 01586 str r3, [r0] */
               goto L_08001d9e;   /* long branch */                                                                 /* 01588 bl #0x8001d9e */
L_0800158c: ;  r1=*(volatile u8*)((r0+12));                                                                         /* 0158c ldrb r1, [r0, #0xc] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0158e cmp r1, #0 */
               if(!Z) goto L_080015ee;                                                                              /* 01590 bne #0x80015ee */
L_08001592: ;  r1=*(volatile u8*)((r0+10));                                                                         /* 01592 ldrb r1, [r0, #0xa] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01594 cmp r1, #0 */
               if(!Z) goto L_0800159c;                                                                              /* 01596 bne #0x800159c */
L_08001598: ;  goto L_08001dc4;   /* long branch */                                                                 /* 01598 bl #0x8001dc4 */
L_0800159c: ;  r0=0x08010935u;                                                                                      /* 0159c ldr r0, [pc, #0x1e0] */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&o_set_message)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* o_set_message */ /* 0159e bl #0x800e854 */
               goto L_08001f2c;   /* long branch */                                                                 /* 015a2 bl #0x8001f2c */
L_080015a6: ;  r3=*(volatile u8*)((r0+10));                                                                         /* 015a6 ldrb r3, [r0, #0xa] */
               { u32 X_=(u32)(r3), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 015a8 cmp r3, #0 */
               if(!Z) goto L_080015ae;                                                                              /* 015aa bne #0x80015ae */
L_080015ac: ;  goto L_08001cf8;                                                                                     /* 015ac b #0x8001cf8 */
L_080015ae: ;  r4=r0; N=(r4)>>31; Z=(r4)==0;                                                                        /* 015ae movs r4, r0 */
               r3=0x0300029Cu;                                                                                      /* 015b0 ldr r3, [pc, #0x1c0] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 015b2 movs r1, #0 */
               *(volatile u32*)(r3)=(u32)r1;                                                                        /* 015b4 str r1, [r3] */
               r3=0x03000298u;                                                                                      /* 015b6 ldr r3, [pc, #0x1c0] */
               *(volatile u32*)(r3)=(u32)r2;                                                                        /* 015b8 str r2, [r3] */
               goto L_08001d94;                                                                                     /* 015ba b #0x8001d94 */
L_080015bc: ;  r1=*(volatile u8*)((r0+13));                                                                         /* 015bc ldrb r1, [r0, #0xd] */
               { u32 X_=(u32)(0u), Y_=(~(u32)(r1)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 015be rsbs r2, r1, #0 */
               { u32 X_=(u32)(r2), Y_=(u32)(r1); unsigned long long W_=(unsigned long long)X_+Y_+(u32)C; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 015c0 adcs r2, r1 */
               *(volatile u8*)((r0+13))=(u8)r2;                                                                     /* 015c2 strb r2, [r0, #0xd] */
               r0=r5; N=(r0)>>31; Z=(r0)==0;                                                                        /* 015c4 movs r0, r5 */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 015c6 cmp r1, #0 */
               if(!Z) goto L_080015cc;                                                                              /* 015c8 bne #0x80015cc */
L_080015ca: ;  goto L_08001be8;                                                                                     /* 015ca b #0x8001be8 */
L_080015cc: ;  r2=0x08010E5Cu;                                                                                      /* 015cc ldr r2, [pc, #0x1b4] */
               goto L_08001bea;                                                                                     /* 015ce b #0x8001bea */
L_080015d0: ;  r1=r0; N=(r1)>>31; Z=(r1)==0;                                                                        /* 015d0 movs r1, r0 */
               r0=0x03000020u;                                                                                      /* 015d2 ldr r0, [pc, #0x170] */
               *(volatile u32*)(r0)=(u32)r2;                                                                        /* 015d4 str r2, [r0] */
               r0=*(volatile u8*)((r1+19));                                                                         /* 015d6 ldrb r0, [r1, #0x13] */
L_080015d8: ;  { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 015d8 cmp r0, #0 */
               if(!Z) goto L_080015e0;                                                                              /* 015da bne #0x80015e0 */
L_080015dc: ;  goto L_08001f2c;   /* long branch */                                                                 /* 015dc bl #0x8001f2c */
L_080015e0: ;  r0=0x03000292u;                                                                                      /* 015e0 ldr r0, [pc, #0x150] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 015e2 movs r1, #0 */
               goto L_08001da8;                                                                                     /* 015e4 b #0x8001da8 */
L_080015e6: ;  r1=*(volatile u8*)((r0+12));                                                                         /* 015e6 ldrb r1, [r0, #0xc] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 015e8 cmp r1, #0 */
               if(!Z) goto L_080015ee;                                                                              /* 015ea bne #0x80015ee */
L_080015ec: ;  goto L_08001d86;                                                                                     /* 015ec b #0x8001d86 */
L_080015ee: ;  r2=0x030002A4u;                                                                                      /* 015ee ldr r2, [pc, #0x180] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 015f0 movs r1, #0 */
               *(volatile u8*)((r2+18))=(u8)r1;                                                                     /* 015f2 strb r1, [r2, #0x12] */
               r3=80u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 015f4 movs r3, #0x50 */
               *(volatile u8*)((r2+17))=(u8)r3;                                                                     /* 015f6 strb r3, [r2, #0x11] */
               r3=69u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 015f8 movs r3, #0x45 */
               *(volatile u8*)((r2+16))=(u8)r3;                                                                     /* 015fa strb r3, [r2, #0x10] */
               *(volatile u8*)((r2+15))=(u8)r3;                                                                     /* 015fc strb r3, [r2, #0xf] */
               r3=76u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 015fe movs r3, #0x4c */
               *(volatile u8*)((r2+14))=(u8)r3;                                                                     /* 01600 strb r3, [r2, #0xe] */
               r3=83u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01602 movs r3, #0x53 */
               *(volatile u8*)((r2+13))=(u8)r3;                                                                     /* 01604 strb r3, [r2, #0xd] */
               r4=65u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 01606 movs r4, #0x41 */
               *(volatile u8*)((r2+12))=(u8)r4;                                                                     /* 01608 strb r4, [r2, #0xc] */
               r4=32u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 0160a movs r4, #0x20 */
               *(volatile u8*)((r2+11))=(u8)r4;                                                                     /* 0160c strb r4, [r2, #0xb] */
               *(volatile u8*)((r2+10))=(u8)r3;                                                                     /* 0160e strb r3, [r2, #0xa] */
               r0=r5; N=(r0)>>31; Z=(r0)==0;                                                                        /* 01610 movs r0, r5 */
               r5=39u; N=(r5)>>31; Z=(r5)==0;                                                                       /* 01612 movs r5, #0x27 */
               *(volatile u8*)((r2+9))=(u8)r5;                                                                      /* 01614 strb r5, [r2, #9] */
               r5=84u; N=(r5)>>31; Z=(r5)==0;                                                                       /* 01616 movs r5, #0x54 */
               *(volatile u8*)((r2+8))=(u8)r5;                                                                      /* 01618 strb r5, [r2, #8] */
               r5=73u; N=(r5)>>31; Z=(r5)==0;                                                                       /* 0161a movs r5, #0x49 */
               *(volatile u8*)((r2+7))=(u8)r5;                                                                      /* 0161c strb r5, [r2, #7] */
               *(volatile u8*)((r2+6))=(u8)r4;                                                                      /* 0161e strb r4, [r2, #6] */
               r4=46u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 01620 movs r4, #0x2e */
               *(volatile u8*)((r2+5))=(u8)r4;                                                                      /* 01622 strb r4, [r2, #5] */
               *(volatile u8*)((r2+4))=(u8)r4;                                                                      /* 01624 strb r4, [r2, #4] */
               *(volatile u8*)((r2+3))=(u8)r4;                                                                      /* 01626 strb r4, [r2, #3] */
               r4=72u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 01628 movs r4, #0x48 */
               *(volatile u8*)((r2+2))=(u8)r4;                                                                      /* 0162a strb r4, [r2, #2] */
               *(volatile u8*)((r2+1))=(u8)r4;                                                                      /* 0162c strb r4, [r2, #1] */
               *(volatile u8*)(r2)=(u8)r3;                                                                          /* 0162e strb r3, [r2] */
               r2=100u; N=(r2)>>31; Z=(r2)==0;                                                                      /* 01630 movs r2, #0x64 */
               *(volatile u32*)(r0)=(u32)r2;                                                                        /* 01632 str r2, [r0] */
               r0=0x03000028u;                                                                                      /* 01634 ldr r0, [pc, #0xf8] */
               r0=*(volatile u8*)((r0+19));                                                                         /* 01636 ldrb r0, [r0, #0x13] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01638 cmp r0, #0 */
               if(!Z) goto L_08001640;                                                                              /* 0163a bne #0x8001640 */
L_0800163c: ;  goto L_08001f2c;   /* long branch */                                                                 /* 0163c bl #0x8001f2c */
L_08001640: ;  r0=0x03000292u;                                                                                      /* 01640 ldr r0, [pc, #0xf0] */
               goto L_080013ce;                                                                                     /* 01642 b #0x80013ce */
L_08001682: ;  r0=0x03000298u;                                                                                      /* 01682 ldr r0, [pc, #0xf4] */
               r0=*(volatile u32*)(r0);                                                                             /* 01684 ldr r0, [r0] */
               r1=48u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01686 movs r1, #0x30 */
               { u32 X_=(u32)(r0), S_=(u32)(2u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r2=R_; N=R_>>31; Z=R_==0; } /* 01688 lsls r2, r0, #2 */
               r3=0x08001690u;                                                                                      /* 0168a adr r3, #4 */
               r2=*(volatile u32*)((r3+r2));                                                                        /* 0168c ldr r2, [r3, r2] */
               switch((r2&~1u)){   case 0x080016C0u: goto L_080016c0;   case 0x080016E6u: goto L_080016e6;   case 0x080016EAu: goto L_080016ea;   case 0x080016EEu: goto L_080016ee;   case 0x080016F2u: goto L_080016f2;   case 0x080016F4u: goto L_080016f4;   default: return; }   // (never: the original has no other case) /* 0168e mov pc, r2 */
L_080016c0: ;  r1=0x0300029Cu;                                                                                      /* 016c0 ldr r1, [pc, #0xb0] */
               r2=*(volatile u32*)(r1);                                                                             /* 016c2 ldr r2, [r1] */
               r1=130u; N=(r1)>>31; Z=(r1)==0;                                                                      /* 016c4 movs r1, #0x82 */
               { u32 X_=(u32)(r2), Y_=(~(u32)(88u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 016c6 cmp r2, #0x58 */
               if(Z) goto L_080016ce;                                                                               /* 016c8 beq #0x80016ce */
L_080016ca: ;  { u32 X_=(u32)(r2), Y_=(~(u32)(44u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 016ca cmp r2, #0x2c */
               if(!Z) goto L_080016f4;                                                                              /* 016cc bne #0x80016f4 */
L_080016ce: ;  r2=0x03000028u;                                                                                      /* 016ce ldr r2, [pc, #0x60] */
               r2=*(volatile u8*)((r2+19));                                                                         /* 016d0 ldrb r2, [r2, #0x13] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 016d2 cmp r2, #0 */
               if(Z) goto L_080016f4;                                                                               /* 016d4 beq #0x80016f4 */
L_080016d6: ;  r2=0x03000292u;                                                                                      /* 016d6 ldr r2, [pc, #0x5c] */
               r3=0u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 016d8 movs r3, #0 */
               *(volatile u8*)(r2)=(u8)r3;                                                                          /* 016da strb r3, [r2] */
               r2=0x0800FE10u;                                                                                      /* 016dc ldr r2, [pc, #0xac] */
               *(volatile u32*)(r6)=(u32)r2;                                                                        /* 016de str r2, [r6] */
               r2=0x03000290u;                                                                                      /* 016e0 ldr r2, [pc, #0x54] */
               *(volatile u8*)(r2)=(u8)r3;                                                                          /* 016e2 strb r3, [r2] */
               goto L_080016f4;                                                                                     /* 016e4 b #0x80016f4 */
L_080016e6: ;  r1=70u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 016e6 movs r1, #0x46 */
               goto L_080016f4;                                                                                     /* 016e8 b #0x80016f4 */
L_080016ea: ;  r1=150u; N=(r1)>>31; Z=(r1)==0;                                                                      /* 016ea movs r1, #0x96 */
               goto L_080016f4;                                                                                     /* 016ec b #0x80016f4 */
L_080016ee: ;  r1=110u; N=(r1)>>31; Z=(r1)==0;                                                                      /* 016ee movs r1, #0x6e */
               goto L_080016f4;                                                                                     /* 016f0 b #0x80016f4 */
L_080016f2: ;  r1=80u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 016f2 movs r1, #0x50 */
L_080016f4: ;  r2=0x0300029Cu;                                                                                      /* 016f4 ldr r2, [pc, #0x7c] */
               r3=*(volatile u32*)(r2);                                                                             /* 016f6 ldr r3, [r2] */
               { u32 X_=(u32)(r3), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r3=R_; } /* 016f8 adds r3, r3, #1 */
               *(volatile u32*)(r2)=(u32)r3;                                                                        /* 016fa str r3, [r2] */
               { u32 X_=(u32)(r3), Y_=(~(u32)(r1)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 016fc cmp r3, r1 */
               if((N==V)) goto L_08001704;                                                                          /* 016fe bge #0x8001704 */
L_08001700: ;  goto L_08001f2c;   /* long branch */                                                                 /* 01700 bl #0x8001f2c */
L_08001704: ;  r2=1u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 01704 movs r2, #1 */
               r1=0x03000020u;                                                                                      /* 01706 ldr r1, [pc, #0x3c] */
               *(volatile u32*)(r1)=(u32)r2;                                                                        /* 01708 str r2, [r1] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(7u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0170a cmp r0, #7 */
               if((!C||Z)) goto L_08001712;                                                                         /* 0170c bls #0x8001712 */
L_0800170e: ;  goto L_08001f2c;   /* long branch */                                                                 /* 0170e bl #0x8001f2c */
L_08001712: ;  r1=0x08001720u;                                                                                      /* 01712 adr r1, #0xc */
               { u32 X_=(u32)(r0), S_=(u32)(1u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r0=R_; N=R_>>31; Z=R_==0; } /* 01714 lsls r0, r0, #1 */
               r0=*(volatile u16*)((r1+r0));                                                                        /* 01716 ldrh r0, [r1, r0] */
               { u32 X_=(u32)(r0), S_=(u32)(1u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r0=R_; N=R_>>31; Z=R_==0; } /* 01718 lsls r0, r0, #1 */
               switch((134223646u+r0)){   case 0x08001790u: goto L_08001790;   case 0x080017BCu: goto L_080017bc;   case 0x08001808u: goto L_08001808;   case 0x080018C6u: goto L_080018c6;   case 0x08001978u: goto L_08001978;   case 0x080019BEu: goto L_080019be;   case 0x080019D2u: goto L_080019d2;   case 0x08001F2Cu: goto L_08001f2c;   default: return; }   // (never: the original has no other case) /* 0171a add pc, r0 */
L_08001790: ;  r0=0x03000028u;                                                                                      /* 01790 ldr r0, [pc, #0x3f0] */
               r1=*(volatile u8*)((r0+6));                                                                          /* 01792 ldrb r1, [r0, #6] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(3u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01794 cmp r1, #3 */
               if((C&&!Z)) goto L_0800179c;                                                                         /* 01796 bhi #0x800179c */
L_08001798: ;  { u32 X_=(u32)(r1), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 01798 adds r1, r1, #1 */
               *(volatile u8*)((r0+6))=(u8)r1;                                                                      /* 0179a strb r1, [r0, #6] */
L_0800179c: ;  r1=*(volatile u8*)((r0+9));                                                                          /* 0179c ldrb r1, [r0, #9] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(98u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0179e cmp r1, #0x62 */
               if((C&&!Z)) goto L_080017a6;                                                                         /* 017a0 bhi #0x80017a6 */
L_080017a2: ;  { u32 X_=(u32)(r1), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 017a2 adds r1, r1, #1 */
               *(volatile u8*)((r0+9))=(u8)r1;                                                                      /* 017a4 strb r1, [r0, #9] */
L_080017a6: ;  r0=0x030002A4u;                                                                                      /* 017a6 ldr r0, [pc, #0x3f4] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 017a8 movs r1, #0 */
               *(volatile u8*)((r0+4))=(u8)r1;                                                                      /* 017aa strb r1, [r0, #4] */
               r1=33u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 017ac movs r1, #0x21 */
               *(volatile u8*)((r0+3))=(u8)r1;                                                                      /* 017ae strb r1, [r0, #3] */
               r1=77u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 017b0 movs r1, #0x4d */
               *(volatile u8*)((r0+2))=(u8)r1;                                                                      /* 017b2 strb r1, [r0, #2] */
               r1=85u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 017b4 movs r1, #0x55 */
               *(volatile u8*)((r0+1))=(u8)r1;                                                                      /* 017b6 strb r1, [r0, #1] */
               r1=89u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 017b8 movs r1, #0x59 */
               goto L_08001b7c;                                                                                     /* 017ba b #0x8001b7c */
L_080017bc: ;  r0=0x03000028u;                                                                                      /* 017bc ldr r0, [pc, #0x3d8] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 017be movs r1, #0 */
               *(volatile u32*)((r0+32))=(u32)r1;                                                                   /* 017c0 str r1, [r0, #0x20] */
               *(volatile u8*)((r0+4))=(u8)r2;                                                                      /* 017c2 strb r2, [r0, #4] */
               r3=5u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 017c4 movs r3, #5 */
               *(volatile u8*)((r0+9))=(u8)r3;                                                                      /* 017c6 strb r3, [r0, #9] */
               { u32 X_=(u32)(sp), Y_=(u32)(36u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; r5=R_; } /* 017c8 add r5, sp, #0x24 */
               r2=32u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 017ca movs r2, #0x20 */
               *(volatile u32*)((sp+28))=(u32)r2;                                                                   /* 017cc str r2, [sp, #0x1c] */
               *(volatile u16*)((r5+12))=(u16)r2;                                                                   /* 017ce strh r2, [r5, #0xc] */
               r4=0x4F54204Fu;                                                                                      /* 017d0 ldr r4, [pc, #0x3cc] */
               *(volatile u32*)((sp+44))=(u32)r4;                                                                   /* 017d2 str r4, [sp, #0x2c] */
               r4=0x4C4C4548u;                                                                                      /* 017d4 ldr r4, [pc, #0x3cc] */
               *(volatile u32*)((sp+40))=(u32)r4;                                                                   /* 017d6 str r4, [sp, #0x28] */
               r4=0x20594153u;                                                                                      /* 017d8 ldr r4, [pc, #0x3cc] */
               *(volatile u32*)((sp+36))=(u32)r4;                                                                   /* 017da str r4, [sp, #0x24] */
               r4=*(volatile u8*)((r0+5));                                                                          /* 017dc ldrb r4, [r0, #5] */
               { u32 X_=(u32)(r4), S_=(u32)(2u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r4=R_; N=R_>>31; Z=R_==0; } /* 017de lsls r4, r4, #2 */
               r6=0x0203D040u;                                                                                      /* 017e0 ldr r6, [pc, #0x3c8] */
               r4=*(volatile u32*)((r6+r4));                                                                        /* 017e2 ldr r4, [r6, r4] */
               r6=*(volatile u8*)(r4);                                                                              /* 017e4 ldrb r6, [r4] */
               { u32 X_=(u32)(r6), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 017e6 cmp r6, #0 */
               *(volatile u32*)((sp+32))=(u32)r3;                                                                   /* 017e8 str r3, [sp, #0x20] */
               if(!Z) goto L_080017ee;                                                                              /* 017ea bne #0x80017ee */
L_080017ec: ;  goto L_08001a78;                                                                                     /* 017ec b #0x8001a78 */
L_080017ee: ;  r5=13u; N=(r5)>>31; Z=(r5)==0;                                                                       /* 017ee movs r5, #0xd */
L_080017f0: ;  { u32 X_=(u32)(sp), Y_=(u32)(36u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; r2=R_; } /* 017f0 add r2, sp, #0x24 */
               *(volatile u8*)((r2+r5))=(u8)r6;                                                                     /* 017f2 strb r6, [r2, r5] */
               { u32 X_=(u32)(r4), Y_=(u32)(r5); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 017f4 adds r2, r4, r5 */
               { u32 X_=(u32)(r2), Y_=(~(u32)(12u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 017f6 subs r2, #0xc */
               r6=*(volatile u8*)(r2);                                                                              /* 017f8 ldrb r6, [r2] */
               { u32 X_=(u32)(r5), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r5=R_; } /* 017fa adds r5, r5, #1 */
               { u32 X_=(u32)(r6), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 017fc cmp r6, #0 */
               if(!Z) goto L_080017f0;                                                                              /* 017fe bne #0x80017f0 */
L_08001800: ;  { u32 X_=(u32)(sp), Y_=(u32)(36u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; r2=R_; } /* 01800 add r2, sp, #0x24 */
               { u32 X_=(u32)(r2), Y_=(u32)(r5); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r5=R_; } /* 01802 adds r5, r2, r5 */
               r4=*(volatile u8*)(r2);                                                                              /* 01804 ldrb r4, [r2] */
               goto L_08001a7c;                                                                                     /* 01806 b #0x8001a7c */
L_08001808: ;  r0=0x03000028u;                                                                                      /* 01808 ldr r0, [pc, #0x38c] */
               r1=*(volatile u8*)((r0+7));                                                                          /* 0180a ldrb r1, [r0, #7] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(3u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0180c cmp r1, #3 */
               if((C&&!Z)) goto L_08001814;                                                                         /* 0180e bhi #0x8001814 */
L_08001810: ;  { u32 X_=(u32)(r1), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 01810 adds r1, r1, #1 */
               *(volatile u8*)((r0+7))=(u8)r1;                                                                      /* 01812 strb r1, [r0, #7] */
L_08001814: ;  r1=*(volatile u8*)((r0+9));                                                                          /* 01814 ldrb r1, [r0, #9] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(97u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01816 cmp r1, #0x61 */
               if((C&&!Z)) goto L_0800181e;                                                                         /* 01818 bhi #0x800181e */
L_0800181a: ;  { u32 X_=(u32)(r1), Y_=(u32)(2u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 0181a adds r1, r1, #2 */
               *(volatile u8*)((r0+9))=(u8)r1;                                                                      /* 0181c strb r1, [r0, #9] */
L_0800181e: ;  r2=66u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 0181e movs r2, #0x42 */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 01820 movs r1, #0 */
               *(volatile u16*)((r0+r2))=(u16)r1;                                                                   /* 01822 strh r1, [r0, r2] */
               r2=*(volatile u8*)((r0+17));                                                                         /* 01824 ldrb r2, [r0, #0x11] */
               { u32 X_=(u32)(r2), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01826 adds r2, r2, #1 */
               *(volatile u8*)((r0+17))=(u8)r2;                                                                     /* 01828 strb r2, [r0, #0x11] */
               r3=255u; N=(r3)>>31; Z=(r3)==0;                                                                      /* 0182a movs r3, #0xff */
               r3=(u32)(r3&(r2)); N=(r3)>>31; Z=(r3)==0;                                                            /* 0182c ands r3, r2 */
               { u32 X_=(u32)(r3), Y_=(~(u32)(4u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0182e cmp r3, #4 */
               if(C) goto L_08001834;                                                                               /* 01830 bhs #0x8001834 */
L_08001832: ;  goto L_08001a22;                                                                                     /* 01832 b #0x8001a22 */
L_08001834: ;  r2=0x03000000u;                                                                                      /* 01834 ldr r2, [pc, #0x378] */
               r2=*(volatile u32*)(r2);                                                                             /* 01836 ldr r2, [r2] */
               { u32 X_=(u32)(r2), S_=(u32)(13u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r3=R_; N=R_>>31; Z=R_==0; } /* 01838 lsls r3, r2, #0xd */
               r3=(u32)(r3^(r2)); N=(r3)>>31; Z=(r3)==0;                                                            /* 0183a eors r3, r2 */
               { u32 X_=(u32)(r3), S_=(u32)(17u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r4=R_; N=R_>>31; Z=R_==0; } /* 0183c lsrs r4, r3, #0x11 */
               r4=(u32)(r4^(r3)); N=(r4)>>31; Z=(r4)==0;                                                            /* 0183e eors r4, r3 */
               { u32 X_=(u32)(r4), S_=(u32)(5u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r2=R_; N=R_>>31; Z=R_==0; } /* 01840 lsls r2, r4, #5 */
               r2=(u32)(r2^(r4)); N=(r2)>>31; Z=(r2)==0;                                                            /* 01842 eors r2, r4 */
               r3=0x03000000u;                                                                                      /* 01844 ldr r3, [pc, #0x368] */
               *(volatile u32*)(r3)=(u32)r2;                                                                        /* 01846 str r2, [r3] */
               if(!N) goto L_0800184c;                                                                              /* 01848 bpl #0x800184c */
L_0800184a: ;  goto L_08001a22;                                                                                     /* 0184a b #0x8001a22 */
L_0800184c: ;  r3=*(volatile u8*)((r0+10));                                                                         /* 0184c ldrb r3, [r0, #0xa] */
               { u32 X_=(u32)(r3), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0184e cmp r3, #0 */
               if(!Z) goto L_08001880;                                                                              /* 01850 bne #0x8001880 */
L_08001852: ;  *(volatile u16*)((r0+60))=(u16)r1;                                                                   /* 01852 strh r1, [r0, #0x3c] */
               { u32 X_=(u32)(r2), S_=(u32)(13u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r3=R_; N=R_>>31; Z=R_==0; } /* 01854 lsls r3, r2, #0xd */
               r3=(u32)(r3^(r2)); N=(r3)>>31; Z=(r3)==0;                                                            /* 01856 eors r3, r2 */
               { u32 X_=(u32)(r3), S_=(u32)(17u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r2=R_; N=R_>>31; Z=R_==0; } /* 01858 lsrs r2, r3, #0x11 */
               r2=(u32)(r2^(r3)); N=(r2)>>31; Z=(r2)==0;                                                            /* 0185a eors r2, r3 */
               { u32 X_=(u32)(r2), S_=(u32)(5u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r3=R_; N=R_>>31; Z=R_==0; } /* 0185c lsls r3, r2, #5 */
               r3=(u32)(r3^(r2)); N=(r3)>>31; Z=(r3)==0;                                                            /* 0185e eors r3, r2 */
               r2=0x03000000u;                                                                                      /* 01860 ldr r2, [pc, #0x34c] */
               *(volatile u32*)(r2)=(u32)r3;                                                                        /* 01862 str r3, [r2] */
               { u32 X_=(u32)(r3), S_=(u32)(31u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r2=R_; N=R_>>31; Z=R_==0; } /* 01864 lsrs r2, r3, #0x1f */
               { u32 X_=(u32)(r2), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01866 adds r2, r2, #1 */
               *(volatile u8*)((r0+10))=(u8)r2;                                                                     /* 01868 strb r2, [r0, #0xa] */
               r2=*(volatile u8*)((r0+19));                                                                         /* 0186a ldrb r2, [r0, #0x13] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0186c cmp r2, #0 */
               if(Z) goto L_0800187c;                                                                               /* 0186e beq #0x800187c */
L_08001870: ;  r2=0x03000292u;                                                                                      /* 01870 ldr r2, [pc, #0x340] */
               *(volatile u8*)(r2)=(u8)r1;                                                                          /* 01872 strb r1, [r2] */
               r2=0x0800FBE0u;                                                                                      /* 01874 ldr r2, [pc, #0x340] */
               *(volatile u32*)(r6)=(u32)r2;                                                                        /* 01876 str r2, [r6] */
               r2=0x03000290u;                                                                                      /* 01878 ldr r2, [pc, #0x340] */
               *(volatile u8*)(r2)=(u8)r1;                                                                          /* 0187a strb r1, [r2] */
L_0800187c: ;  r2=70u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 0187c movs r2, #0x46 */
               *(volatile u16*)((r0+r2))=(u16)r1;                                                                   /* 0187e strh r1, [r0, r2] */
L_08001880: ;  r0=0x030002A4u;                                                                                      /* 01880 ldr r0, [pc, #0x318] */
               *(volatile u8*)((r0+20))=(u8)r1;                                                                     /* 01882 strb r1, [r0, #0x14] */
               r1=33u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01884 movs r1, #0x21 */
               *(volatile u8*)((r0+19))=(u8)r1;                                                                     /* 01886 strb r1, [r0, #0x13] */
               r1=69u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01888 movs r1, #0x45 */
               *(volatile u8*)((r0+18))=(u8)r1;                                                                     /* 0188a strb r1, [r0, #0x12] */
               r1=72u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 0188c movs r1, #0x48 */
               *(volatile u8*)((r0+17))=(u8)r1;                                                                     /* 0188e strb r1, [r0, #0x11] */
               r2=67u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 01890 movs r2, #0x43 */
               *(volatile u8*)((r0+16))=(u8)r2;                                                                     /* 01892 strb r2, [r0, #0x10] */
               r2=65u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 01894 movs r2, #0x41 */
               *(volatile u8*)((r0+15))=(u8)r2;                                                                     /* 01896 strb r2, [r0, #0xf] */
               r2=32u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 01898 movs r2, #0x20 */
               *(volatile u8*)((r0+14))=(u8)r2;                                                                     /* 0189a strb r2, [r0, #0xe] */
               r3=89u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 0189c movs r3, #0x59 */
               *(volatile u8*)((r0+13))=(u8)r3;                                                                     /* 0189e strb r3, [r0, #0xd] */
               r3=77u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 018a0 movs r3, #0x4d */
               *(volatile u8*)((r0+12))=(u8)r3;                                                                     /* 018a2 strb r3, [r0, #0xc] */
               *(volatile u8*)((r0+11))=(u8)r3;                                                                     /* 018a4 strb r3, [r0, #0xb] */
               r3=85u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 018a6 movs r3, #0x55 */
               *(volatile u8*)((r0+10))=(u8)r3;                                                                     /* 018a8 strb r3, [r0, #0xa] */
               r4=84u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 018aa movs r4, #0x54 */
               *(volatile u8*)((r0+9))=(u8)r4;                                                                      /* 018ac strb r4, [r0, #9] */
               *(volatile u8*)((r0+8))=(u8)r2;                                                                      /* 018ae strb r2, [r0, #8] */
               r4=46u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 018b0 movs r4, #0x2e */
               *(volatile u8*)((r0+7))=(u8)r4;                                                                      /* 018b2 strb r4, [r0, #7] */
               *(volatile u8*)((r0+6))=(u8)r4;                                                                      /* 018b4 strb r4, [r0, #6] */
               *(volatile u8*)((r0+5))=(u8)r4;                                                                      /* 018b6 strb r4, [r0, #5] */
               *(volatile u8*)((r0+4))=(u8)r1;                                                                      /* 018b8 strb r1, [r0, #4] */
               r4=79u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 018ba movs r4, #0x4f */
               *(volatile u8*)((r0+3))=(u8)r4;                                                                      /* 018bc strb r4, [r0, #3] */
               *(volatile u8*)((r0+2))=(u8)r2;                                                                      /* 018be strb r2, [r0, #2] */
               *(volatile u8*)((r0+1))=(u8)r1;                                                                      /* 018c0 strb r1, [r0, #1] */
               *(volatile u8*)(r0)=(u8)r3;                                                                          /* 018c2 strb r3, [r0] */
               goto L_08001b7e;                                                                                     /* 018c4 b #0x8001b7e */
L_080018c6: ;  r0=0x03000028u;                                                                                      /* 018c6 ldr r0, [pc, #0x2d0] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 018c8 movs r1, #0 */
               *(volatile u32*)((r0+32))=(u32)r1;                                                                   /* 018ca str r1, [r0, #0x20] */
               *(volatile u8*)((r0+15))=(u8)r1;                                                                     /* 018cc strb r1, [r0, #0xf] */
               r2=0x030002A0u;                                                                                      /* 018ce ldr r2, [pc, #0x2f0] */
               r2=*(volatile u32*)(r2);                                                                             /* 018d0 ldr r2, [r2] */
               *(volatile u8*)((r0+4))=(u8)r2;                                                                      /* 018d2 strb r2, [r0, #4] */
               r3=255u; N=(r3)>>31; Z=(r3)==0;                                                                      /* 018d4 movs r3, #0xff */
               r3=(u32)(r3&(r2)); N=(r3)>>31; Z=(r3)==0;                                                            /* 018d6 ands r3, r2 */
               r2=0x0800FE20u;                                                                                      /* 018d8 ldr r2, [pc, #0x2e8] */
               r2=*(volatile u8*)((r2+r3));                                                                         /* 018da ldrb r2, [r2, r3] */
               r3=*(volatile u8*)((r0+9));                                                                          /* 018dc ldrb r3, [r0, #9] */
               { u32 X_=(u32)(r3), Y_=(~(u32)(r2)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 018de cmp r3, r2 */
               if(C) goto L_080018e4;                                                                               /* 018e0 bhs #0x80018e4 */
L_080018e2: ;  *(volatile u8*)((r0+9))=(u8)r2;                                                                      /* 018e2 strb r2, [r0, #9] */
L_080018e4: ;  r2=*(volatile u8*)((r0+5));                                                                          /* 018e4 ldrb r2, [r0, #5] */
               { u32 X_=(u32)(r2), S_=(u32)(2u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r2=R_; N=R_>>31; Z=R_==0; } /* 018e6 lsls r2, r2, #2 */
               r3=0x0203D040u;                                                                                      /* 018e8 ldr r3, [pc, #0x2c0] */
               r4=*(volatile u32*)((r3+r2));                                                                        /* 018ea ldr r4, [r3, r2] */
               r3=*(volatile u8*)(r4);                                                                              /* 018ec ldrb r3, [r4] */
               { u32 X_=(u32)(sp), Y_=(u32)(76u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; r2=R_; } /* 018ee add r2, sp, #0x4c */
               { u32 X_=(u32)(r3), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 018f0 cmp r3, #0 */
               if(Z) goto L_08001904;                                                                               /* 018f2 beq #0x8001904 */
L_080018f4: ;  { u32 X_=(u32)(r4), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r4=R_; } /* 018f4 adds r4, r4, #1 */
               { u32 X_=(u32)(sp), Y_=(u32)(76u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; r2=R_; } /* 018f6 add r2, sp, #0x4c */
L_080018f8: ;  *(volatile u8*)(r2)=(u8)r3;                                                                          /* 018f8 strb r3, [r2] */
               r3=*(volatile u8*)(r4);                                                                              /* 018fa ldrb r3, [r4] */
               { u32 X_=(u32)(r4), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r4=R_; } /* 018fc adds r4, r4, #1 */
               { u32 X_=(u32)(r2), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 018fe adds r2, r2, #1 */
               { u32 X_=(u32)(r3), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01900 cmp r3, #0 */
               if(!Z) goto L_080018f8;                                                                              /* 01902 bne #0x80018f8 */
L_08001904: ;  r3=71u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01904 movs r3, #0x47 */
               *(volatile u8*)((r2+1))=(u8)r3;                                                                      /* 01906 strb r3, [r2, #1] */
               r3=69u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01908 movs r3, #0x45 */
               *(volatile u8*)((r2+3))=(u8)r3;                                                                      /* 0190a strb r3, [r2, #3] */
               r3=82u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 0190c movs r3, #0x52 */
               *(volatile u8*)((r2+2))=(u8)r3;                                                                      /* 0190e strb r3, [r2, #2] */
               *(volatile u8*)((r2+10))=(u8)r1;                                                                     /* 01910 strb r1, [r2, #0xa] */
               r3=79u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01912 movs r3, #0x4f */
               *(volatile u8*)((r2+9))=(u8)r3;                                                                      /* 01914 strb r3, [r2, #9] */
               r3=84u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01916 movs r3, #0x54 */
               *(volatile u8*)((r2+8))=(u8)r3;                                                                      /* 01918 strb r3, [r2, #8] */
               r3=78u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 0191a movs r3, #0x4e */
               *(volatile u8*)((r2+7))=(u8)r3;                                                                      /* 0191c strb r3, [r2, #7] */
               r3=73u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 0191e movs r3, #0x49 */
               *(volatile u8*)((r2+6))=(u8)r3;                                                                      /* 01920 strb r3, [r2, #6] */
               r3=32u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01922 movs r3, #0x20 */
               *(volatile u8*)((r2+5))=(u8)r3;                                                                      /* 01924 strb r3, [r2, #5] */
               r4=87u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 01926 movs r4, #0x57 */
               *(volatile u8*)((r2+4))=(u8)r4;                                                                      /* 01928 strb r4, [r2, #4] */
               *(volatile u8*)(r2)=(u8)r3;                                                                          /* 0192a strb r3, [r2] */
               { u32 X_=(u32)(sp), Y_=(u32)(36u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; r2=R_; } /* 0192c add r2, sp, #0x24 */
               *(volatile u8*)((r2+1))=(u8)r3;                                                                      /* 0192e strb r3, [r2, #1] */
               r3=65u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01930 movs r3, #0x41 */
               *(volatile u8*)(r2)=(u8)r3;                                                                          /* 01932 strb r3, [r2] */
               *(volatile u8*)((r2+2))=(u8)r1;                                                                      /* 01934 strb r1, [r2, #2] */
               r3=*(volatile u8*)((r0+4));                                                                          /* 01936 ldrb r3, [r0, #4] */
               { u32 X_=(u32)(r3), S_=(u32)(2u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r3=R_; N=R_>>31; Z=R_==0; } /* 01938 lsls r3, r3, #2 */
               r4=0x0203D068u;                                                                                      /* 0193a ldr r4, [pc, #0x28c] */
               r4=*(volatile u32*)((r4+r3));                                                                        /* 0193c ldr r4, [r4, r3] */
               r3=*(volatile u8*)(r4);                                                                              /* 0193e ldrb r3, [r4] */
               { u32 X_=(u32)(r2), Y_=(u32)(2u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01940 adds r2, r2, #2 */
               { u32 X_=(u32)(r3), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01942 cmp r3, #0 */
               if(Z) goto L_08001954;                                                                               /* 01944 beq #0x8001954 */
L_08001946: ;  { u32 X_=(u32)(r4), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r4=R_; } /* 01946 adds r4, r4, #1 */
L_08001948: ;  *(volatile u8*)(r2)=(u8)r3;                                                                          /* 01948 strb r3, [r2] */
               r3=*(volatile u8*)(r4);                                                                              /* 0194a ldrb r3, [r4] */
               { u32 X_=(u32)(r4), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r4=R_; } /* 0194c adds r4, r4, #1 */
               { u32 X_=(u32)(r2), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 0194e adds r2, r2, #1 */
               { u32 X_=(u32)(r3), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01950 cmp r3, #0 */
               if(!Z) goto L_08001948;                                                                              /* 01952 bne #0x8001948 */
L_08001954: ;  *(volatile u8*)((r2+1))=(u8)r1;                                                                      /* 01954 strb r1, [r2, #1] */
               r3=33u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01956 movs r3, #0x21 */
               *(volatile u8*)(r2)=(u8)r3;                                                                          /* 01958 strb r3, [r2] */
               { u32 X_=(u32)(sp), Y_=(u32)(76u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; r2=R_; } /* 0195a add r2, sp, #0x4c */
               r2=*(volatile u8*)(r2);                                                                              /* 0195c ldrb r2, [r2] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 0195e cmp r2, #0 */
               if(!Z) goto L_08001964;                                                                              /* 01960 bne #0x8001964 */
L_08001962: ;  goto L_08001ae4;                                                                                     /* 01962 b #0x8001ae4 */
L_08001964: ;  { u32 X_=(u32)(sp), Y_=(u32)(76u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; r3=R_; } /* 01964 add r3, sp, #0x4c */
               { u32 X_=(u32)(r3), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r4=R_; } /* 01966 adds r4, r3, #1 */
               r3=0x03000359u;                                                                                      /* 01968 ldr r3, [pc, #0x260] */
L_0800196a: ;  *(volatile u8*)(r3)=(u8)r2;                                                                          /* 0196a strb r2, [r3] */
               r2=*(volatile u8*)(r4);                                                                              /* 0196c ldrb r2, [r4] */
               { u32 X_=(u32)(r4), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r4=R_; } /* 0196e adds r4, r4, #1 */
               { u32 X_=(u32)(r3), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r3=R_; } /* 01970 adds r3, r3, #1 */
               { u32 X_=(u32)(r2), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01972 cmp r2, #0 */
               if(!Z) goto L_0800196a;                                                                              /* 01974 bne #0x800196a */
L_08001976: ;  goto L_08001ae6;                                                                                     /* 01976 b #0x8001ae6 */
L_08001978: ;  r0=0x03000028u;                                                                                      /* 01978 ldr r0, [pc, #0x21c] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 0197a movs r1, #0 */
               *(volatile u16*)((r0+58))=(u16)r1;                                                                   /* 0197c strh r1, [r0, #0x3a] */
               *(volatile u8*)((r0+11))=(u8)r1;                                                                     /* 0197e strb r1, [r0, #0xb] */
               r0=0x030002A4u;                                                                                      /* 01980 ldr r0, [pc, #0x218] */
               *(volatile u8*)((r0+16))=(u8)r1;                                                                     /* 01982 strb r1, [r0, #0x10] */
               r1=33u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01984 movs r1, #0x21 */
               *(volatile u8*)((r0+15))=(u8)r1;                                                                     /* 01986 strb r1, [r0, #0xf] */
               r2=78u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 01988 movs r2, #0x4e */
               *(volatile u8*)((r0+14))=(u8)r2;                                                                     /* 0198a strb r2, [r0, #0xe] */
               r1=65u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 0198c movs r1, #0x41 */
               *(volatile u8*)((r0+13))=(u8)r1;                                                                     /* 0198e strb r1, [r0, #0xd] */
               r3=69u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01990 movs r3, #0x45 */
               *(volatile u8*)((r0+12))=(u8)r3;                                                                     /* 01992 strb r3, [r0, #0xc] */
               r3=76u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01994 movs r3, #0x4c */
               *(volatile u8*)((r0+11))=(u8)r3;                                                                     /* 01996 strb r3, [r0, #0xb] */
               r4=67u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 01998 movs r4, #0x43 */
               *(volatile u8*)((r0+10))=(u8)r4;                                                                     /* 0199a strb r4, [r0, #0xa] */
               r4=32u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 0199c movs r4, #0x20 */
               *(volatile u8*)((r0+9))=(u8)r4;                                                                      /* 0199e strb r4, [r0, #9] */
               r4=71u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 019a0 movs r4, #0x47 */
               *(volatile u8*)((r0+8))=(u8)r4;                                                                      /* 019a2 strb r4, [r0, #8] */
               *(volatile u8*)((r0+7))=(u8)r2;                                                                      /* 019a4 strb r2, [r0, #7] */
               r2=73u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 019a6 movs r2, #0x49 */
               *(volatile u8*)((r0+6))=(u8)r2;                                                                      /* 019a8 strb r2, [r0, #6] */
               *(volatile u8*)((r0+5))=(u8)r3;                                                                      /* 019aa strb r3, [r0, #5] */
               r2=75u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 019ac movs r2, #0x4b */
               *(volatile u8*)((r0+4))=(u8)r2;                                                                      /* 019ae strb r2, [r0, #4] */
               r2=82u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 019b0 movs r2, #0x52 */
               *(volatile u8*)((r0+3))=(u8)r2;                                                                      /* 019b2 strb r2, [r0, #3] */
               *(volatile u8*)((r0+2))=(u8)r1;                                                                      /* 019b4 strb r1, [r0, #2] */
               r1=80u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 019b6 movs r1, #0x50 */
               *(volatile u8*)((r0+1))=(u8)r1;                                                                      /* 019b8 strb r1, [r0, #1] */
               r1=83u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 019ba movs r1, #0x53 */
               goto L_08001b7c;                                                                                     /* 019bc b #0x8001b7c */
L_080019be: ;  r0=0x03000028u;                                                                                      /* 019be ldr r0, [pc, #0x1d8] */
               r1=*(volatile u8*)((r0+10));                                                                         /* 019c0 ldrb r1, [r0, #0xa] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 019c2 cmp r1, #0 */
               if(Z) goto L_08001a3c;                                                                               /* 019c4 beq #0x8001a3c */
L_080019c6: ;  { u32 X_=(u32)(r1), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 019c6 subs r1, r1, #1 */
               *(volatile u8*)((r0+10))=(u8)r1;                                                                     /* 019c8 strb r1, [r0, #0xa] */
               { u32 X_=(u32)(r1), S_=(u32)(24u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 019ca lsls r1, r1, #0x18 */
               if(Z) goto L_08001a3c;                                                                               /* 019cc beq #0x8001a3c */
L_080019ce: ;  r1=0x080108A2u;                                                                                      /* 019ce ldr r1, [pc, #0x200] */
               goto L_08001a3e;                                                                                     /* 019d0 b #0x8001a3e */
L_080019d2: ;  r0=0x03000028u;                                                                                      /* 019d2 ldr r0, [pc, #0x1c4] */
               r1=*(volatile u8*)((r0+14));                                                                         /* 019d4 ldrb r1, [r0, #0xe] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 019d6 cmp r1, #0 */
               if(!Z) goto L_080019dc;                                                                              /* 019d8 bne #0x80019dc */
L_080019da: ;  goto L_08001b28;                                                                                     /* 019da b #0x8001b28 */
L_080019dc: ;  r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 019dc movs r1, #0 */
               *(volatile u8*)((r0+14))=(u8)r1;                                                                     /* 019de strb r1, [r0, #0xe] */
               r2=*(volatile u8*)((r0+8));                                                                          /* 019e0 ldrb r2, [r0, #8] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(3u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 019e2 cmp r2, #3 */
               if((C&&!Z)) goto L_080019ea;                                                                         /* 019e4 bhi #0x80019ea */
L_080019e6: ;  { u32 X_=(u32)(r2), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 019e6 adds r2, r2, #1 */
               *(volatile u8*)((r0+8))=(u8)r2;                                                                      /* 019e8 strb r2, [r0, #8] */
L_080019ea: ;  r0=0x030002A4u;                                                                                      /* 019ea ldr r0, [pc, #0x1b0] */
               *(volatile u8*)((r0+15))=(u8)r1;                                                                     /* 019ec strb r1, [r0, #0xf] */
               r1=33u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 019ee movs r1, #0x21 */
               *(volatile u8*)((r0+14))=(u8)r1;                                                                     /* 019f0 strb r1, [r0, #0xe] */
               r1=68u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 019f2 movs r1, #0x44 */
               *(volatile u8*)((r0+13))=(u8)r1;                                                                     /* 019f4 strb r1, [r0, #0xd] */
               r1=69u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 019f6 movs r1, #0x45 */
               *(volatile u8*)((r0+12))=(u8)r1;                                                                     /* 019f8 strb r1, [r0, #0xc] */
               r2=86u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 019fa movs r2, #0x56 */
               *(volatile u8*)((r0+11))=(u8)r2;                                                                     /* 019fc strb r2, [r0, #0xb] */
               r2=79u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 019fe movs r2, #0x4f */
               *(volatile u8*)((r0+10))=(u8)r2;                                                                     /* 01a00 strb r2, [r0, #0xa] */
               r2=76u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 01a02 movs r2, #0x4c */
               *(volatile u8*)((r0+9))=(u8)r2;                                                                      /* 01a04 strb r2, [r0, #9] */
               r3=32u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01a06 movs r3, #0x20 */
               *(volatile u8*)((r0+8))=(u8)r3;                                                                      /* 01a08 strb r3, [r0, #8] */
               r4=83u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 01a0a movs r4, #0x53 */
               *(volatile u8*)((r0+7))=(u8)r4;                                                                      /* 01a0c strb r4, [r0, #7] */
               *(volatile u8*)((r0+6))=(u8)r2;                                                                      /* 01a0e strb r2, [r0, #6] */
               *(volatile u8*)((r0+5))=(u8)r1;                                                                      /* 01a10 strb r1, [r0, #5] */
               *(volatile u8*)((r0+4))=(u8)r1;                                                                      /* 01a12 strb r1, [r0, #4] */
               r1=70u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01a14 movs r1, #0x46 */
               *(volatile u8*)((r0+3))=(u8)r1;                                                                      /* 01a16 strb r1, [r0, #3] */
               *(volatile u8*)((r0+2))=(u8)r3;                                                                      /* 01a18 strb r3, [r0, #2] */
               r1=84u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01a1a movs r1, #0x54 */
               *(volatile u8*)((r0+1))=(u8)r1;                                                                      /* 01a1c strb r1, [r0, #1] */
               r1=73u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01a1e movs r1, #0x49 */
               goto L_08001b7c;                                                                                     /* 01a20 b #0x8001b7c */
L_08001a22: ;  r0=0x030002A4u;                                                                                      /* 01a22 ldr r0, [pc, #0x178] */
               *(volatile u8*)((r0+6))=(u8)r1;                                                                      /* 01a24 strb r1, [r0, #6] */
               r1=33u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01a26 movs r1, #0x21 */
               *(volatile u8*)((r0+5))=(u8)r1;                                                                      /* 01a28 strb r1, [r0, #5] */
               r1=89u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01a2a movs r1, #0x59 */
               *(volatile u8*)((r0+4))=(u8)r1;                                                                      /* 01a2c strb r1, [r0, #4] */
               r1=84u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01a2e movs r1, #0x54 */
               *(volatile u8*)((r0+3))=(u8)r1;                                                                      /* 01a30 strb r1, [r0, #3] */
               r2=83u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 01a32 movs r2, #0x53 */
               *(volatile u8*)((r0+2))=(u8)r2;                                                                      /* 01a34 strb r2, [r0, #2] */
               r2=65u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 01a36 movs r2, #0x41 */
               *(volatile u8*)((r0+1))=(u8)r2;                                                                      /* 01a38 strb r2, [r0, #1] */
               goto L_08001b7c;                                                                                     /* 01a3a b #0x8001b7c */
L_08001a3c: ;  r1=0x0801134Au;                                                                                      /* 01a3c ldr r1, [pc, #0x194] */
L_08001a3e: ;  r2=*(volatile u16*)((r0+22));                                                                        /* 01a3e ldrh r2, [r0, #0x16] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(90u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01a40 cmp r2, #0x5a */
               if(!C) goto L_08001a46;                                                                              /* 01a42 blo #0x8001a46 */
L_08001a44: ;  r2=90u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 01a44 movs r2, #0x5a */
L_08001a46: ;  { u32 X_=(u32)(r2), Y_=(u32)(10u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01a46 adds r2, #0xa */
               *(volatile u16*)((r0+22))=(u16)r2;                                                                   /* 01a48 strh r2, [r0, #0x16] */
               r0=*(volatile u8*)(r1);                                                                              /* 01a4a ldrb r0, [r1] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01a4c cmp r0, #0 */
               if(Z) goto L_08001a6c;                                                                               /* 01a4e beq #0x8001a6c */
L_08001a50: ;  { u32 X_=(u32)(r1), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 01a50 adds r1, r1, #1 */
               r3=0u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 01a52 movs r3, #0 */
               r2=0x030002A4u;                                                                                      /* 01a54 ldr r2, [pc, #0x144] */
L_08001a56: ;  *(volatile u8*)((r2+r3))=(u8)r0;                                                                     /* 01a56 strb r0, [r2, r3] */
               r0=*(volatile u8*)((r1+r3));                                                                         /* 01a58 ldrb r0, [r1, r3] */
               { u32 X_=(u32)(r3), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r4=R_; } /* 01a5a adds r4, r3, #1 */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01a5c cmp r0, #0 */
               if(Z) goto L_08001a68;                                                                               /* 01a5e beq #0x8001a68 */
L_08001a60: ;  { u32 X_=(u32)(r3), Y_=(~(u32)(37u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01a60 cmp r3, #0x25 */
               sp-=4; *(volatile u32*)(sp+0)=r4;                                                                    /* 01a62 push {r4} */
               r3=*(volatile u32*)(sp+0); sp+=4;                                                                    /* 01a64 pop {r3} */
               if(!C) goto L_08001a56;                                                                              /* 01a66 blo #0x8001a56 */
L_08001a68: ;  { u32 X_=(u32)(r2), Y_=(u32)(r4); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 01a68 adds r0, r2, r4 */
               goto L_08001a6e;                                                                                     /* 01a6a b #0x8001a6e */
L_08001a6c: ;  r0=0x030002A4u;                                                                                      /* 01a6c ldr r0, [pc, #0x12c] */
L_08001a6e: ;  r1=100u; N=(r1)>>31; Z=(r1)==0;                                                                      /* 01a6e movs r1, #0x64 */
               *(volatile u32*)(r5)=(u32)r1;                                                                        /* 01a70 str r1, [r5] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 01a72 movs r1, #0 */
               *(volatile u8*)(r0)=(u8)r1;                                                                          /* 01a74 strb r1, [r0] */
               goto L_08001f2c;                                                                                     /* 01a76 b #0x8001f2c */
L_08001a78: ;  { u32 X_=(u32)(r5), Y_=(u32)(13u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r5=R_; } /* 01a78 adds r5, #0xd */
               r4=83u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 01a7a movs r4, #0x53 */
L_08001a7c: ;  *(volatile u8*)((r5+1))=(u8)r1;                                                                      /* 01a7c strb r1, [r5, #1] */
               r2=33u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 01a7e movs r2, #0x21 */
               *(volatile u8*)(r5)=(u8)r2;                                                                          /* 01a80 strb r2, [r5] */
               r5=0x03000359u;                                                                                      /* 01a82 ldr r5, [pc, #0x148] */
               *(volatile u8*)((r5+11))=(u8)r1;                                                                     /* 01a84 strb r1, [r5, #0xb] */
               *(volatile u8*)((r5+10))=(u8)r2;                                                                     /* 01a86 strb r2, [r5, #0xa] */
               r2=68u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 01a88 movs r2, #0x44 */
               *(volatile u8*)((r5+9))=(u8)r2;                                                                      /* 01a8a strb r2, [r5, #9] */
               r2=69u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 01a8c movs r2, #0x45 */
               *(volatile u8*)((r5+8))=(u8)r2;                                                                      /* 01a8e strb r2, [r5, #8] */
               r2=72u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 01a90 movs r2, #0x48 */
               *(volatile u8*)((r5+7))=(u8)r2;                                                                      /* 01a92 strb r2, [r5, #7] */
               r6=67u; N=(r6)>>31; Z=(r6)==0;                                                                       /* 01a94 movs r6, #0x43 */
               *(volatile u8*)((r5+6))=(u8)r6;                                                                      /* 01a96 strb r6, [r5, #6] */
               r6=84u; N=(r6)>>31; Z=(r6)==0;                                                                       /* 01a98 movs r6, #0x54 */
               *(volatile u8*)((r5+5))=(u8)r6;                                                                      /* 01a9a strb r6, [r5, #5] */
               r3=65u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01a9c movs r3, #0x41 */
               *(volatile u8*)((r5+4))=(u8)r3;                                                                      /* 01a9e strb r3, [r5, #4] */
               *(volatile u8*)((r5+3))=(u8)r2;                                                                      /* 01aa0 strb r2, [r5, #3] */
               r2=*(volatile u32*)((sp+28));                                                                        /* 01aa2 ldr r2, [sp, #0x1c] */
               *(volatile u8*)((r5+2))=(u8)r2;                                                                      /* 01aa4 strb r2, [r5, #2] */
               *(volatile u8*)((r5+1))=(u8)r6;                                                                      /* 01aa6 strb r6, [r5, #1] */
               r2=73u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 01aa8 movs r2, #0x49 */
               *(volatile u8*)(r5)=(u8)r2;                                                                          /* 01aaa strb r2, [r5] */
               { u32 X_=(u32)(r4), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01aac cmp r4, #0 */
               if(Z) goto L_08001ac4;                                                                               /* 01aae beq #0x8001ac4 */
L_08001ab0: ;  { u32 X_=(u32)(sp), Y_=(u32)(36u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; r2=R_; } /* 01ab0 add r2, sp, #0x24 */
               { u32 X_=(u32)(r2), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r5=R_; } /* 01ab2 adds r5, r2, #1 */
               r3=0x03000381u;                                                                                      /* 01ab4 ldr r3, [pc, #0x120] */
L_08001ab6: ;  *(volatile u8*)(r3)=(u8)r4;                                                                          /* 01ab6 strb r4, [r3] */
               r4=*(volatile u8*)(r5);                                                                              /* 01ab8 ldrb r4, [r5] */
               { u32 X_=(u32)(r5), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r5=R_; } /* 01aba adds r5, r5, #1 */
               { u32 X_=(u32)(r3), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r3=R_; } /* 01abc adds r3, r3, #1 */
               { u32 X_=(u32)(r4), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01abe cmp r4, #0 */
               if(!Z) goto L_08001ab6;                                                                              /* 01ac0 bne #0x8001ab6 */
L_08001ac2: ;  goto L_08001ac6;                                                                                     /* 01ac2 b #0x8001ac6 */
L_08001ac4: ;  r3=0x03000381u;                                                                                      /* 01ac4 ldr r3, [pc, #0x110] */
L_08001ac6: ;  r2=0x03000020u;                                                                                      /* 01ac6 ldr r2, [pc, #0x114] */
               r4=*(volatile u32*)((sp+32));                                                                        /* 01ac8 ldr r4, [sp, #0x20] */
               *(volatile u32*)(r2)=(u32)r4;                                                                        /* 01aca str r4, [r2] */
               *(volatile u8*)(r3)=(u8)r1;                                                                          /* 01acc strb r1, [r3] */
               r2=0x31504B43u;                                                                                      /* 01ace ldr r2, [pc, #0x110] */
               *(volatile u32*)(r0)=(u32)r2;                                                                        /* 01ad0 str r2, [r0] */
               r2=0x00005EEDu;                                                                                      /* 01ad2 ldr r2, [pc, #0x110] */
               r3=31u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01ad4 movs r3, #0x1f */
L_08001ad6: ;  r4=*(volatile u8*)((r0+r1));                                                                         /* 01ad6 ldrb r4, [r0, r1] */
               r2=(u32)(r3*(r2)); N=(r2)>>31; Z=(r2)==0;                                                            /* 01ad8 muls r2, r3, r2 */
               { u32 X_=(u32)(r2), Y_=(u32)(r4); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01ada adds r2, r2, r4 */
               { u32 X_=(u32)(r1), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 01adc adds r1, r1, #1 */
               { u32 X_=(u32)(r1), Y_=(~(u32)(72u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01ade cmp r1, #0x48 */
               if(!Z) goto L_08001ad6;                                                                              /* 01ae0 bne #0x8001ad6 */
L_08001ae2: ;  goto L_08001b22;                                                                                     /* 01ae2 b #0x8001b22 */
L_08001ae4: ;  r3=0x03000359u;                                                                                      /* 01ae4 ldr r3, [pc, #0xe4] */
L_08001ae6: ;  *(volatile u8*)(r3)=(u8)r1;                                                                          /* 01ae6 strb r1, [r3] */
               { u32 X_=(u32)(sp), Y_=(u32)(36u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; r2=R_; } /* 01ae8 add r2, sp, #0x24 */
               r3=*(volatile u8*)(r2);                                                                              /* 01aea ldrb r3, [r2] */
               { u32 X_=(u32)(r3), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01aec cmp r3, #0 */
               if(Z) goto L_08001b04;                                                                               /* 01aee beq #0x8001b04 */
L_08001af0: ;  { u32 X_=(u32)(sp), Y_=(u32)(36u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; r2=R_; } /* 01af0 add r2, sp, #0x24 */
               { u32 X_=(u32)(r2), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r4=R_; } /* 01af2 adds r4, r2, #1 */
               r2=0x03000381u;                                                                                      /* 01af4 ldr r2, [pc, #0xe0] */
L_08001af6: ;  *(volatile u8*)(r2)=(u8)r3;                                                                          /* 01af6 strb r3, [r2] */
               r3=*(volatile u8*)(r4);                                                                              /* 01af8 ldrb r3, [r4] */
               { u32 X_=(u32)(r4), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r4=R_; } /* 01afa adds r4, r4, #1 */
               { u32 X_=(u32)(r2), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01afc adds r2, r2, #1 */
               { u32 X_=(u32)(r3), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01afe cmp r3, #0 */
               if(!Z) goto L_08001af6;                                                                              /* 01b00 bne #0x8001af6 */
L_08001b02: ;  goto L_08001b06;                                                                                     /* 01b02 b #0x8001b06 */
L_08001b04: ;  r2=0x03000381u;                                                                                      /* 01b04 ldr r2, [pc, #0xd0] */
L_08001b06: ;  r3=5u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 01b06 movs r3, #5 */
               r4=0x03000020u;                                                                                      /* 01b08 ldr r4, [pc, #0xd0] */
               *(volatile u32*)(r4)=(u32)r3;                                                                        /* 01b0a str r3, [r4] */
               *(volatile u8*)(r2)=(u8)r1;                                                                          /* 01b0c strb r1, [r2] */
               r2=0x31504B43u;                                                                                      /* 01b0e ldr r2, [pc, #0xd0] */
               *(volatile u32*)(r0)=(u32)r2;                                                                        /* 01b10 str r2, [r0] */
               r2=0x00005EEDu;                                                                                      /* 01b12 ldr r2, [pc, #0xd0] */
               r3=31u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01b14 movs r3, #0x1f */
L_08001b16: ;  r4=*(volatile u8*)((r0+r1));                                                                         /* 01b16 ldrb r4, [r0, r1] */
               r2=(u32)(r3*(r2)); N=(r2)>>31; Z=(r2)==0;                                                            /* 01b18 muls r2, r3, r2 */
               { u32 X_=(u32)(r2), Y_=(u32)(r4); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01b1a adds r2, r2, r4 */
               { u32 X_=(u32)(r1), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 01b1c adds r1, r1, #1 */
               { u32 X_=(u32)(r1), Y_=(~(u32)(72u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01b1e cmp r1, #0x48 */
               if(!Z) goto L_08001b16;                                                                              /* 01b20 bne #0x8001b16 */
L_08001b22: ;  *(volatile u32*)((r0+72))=(u32)r2;                                                                   /* 01b22 str r2, [r0, #0x48] */
               goto L_08000c8c;   /* long branch */                                                                 /* 01b24 bl #0x8000c8c */
L_08001b28: ;  r1=64u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01b28 movs r1, #0x40 */
               r2=*(volatile u16*)((r0+r1));                                                                        /* 01b2a ldrh r2, [r0, r1] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01b2c cmp r2, #0 */
               if(Z) goto L_08001b54;                                                                               /* 01b2e beq #0x8001b54 */
L_08001b30: ;  r0=0x030002A4u;                                                                                      /* 01b30 ldr r0, [pc, #0x68] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 01b32 movs r1, #0 */
               *(volatile u8*)((r0+7))=(u8)r1;                                                                      /* 01b34 strb r1, [r0, #7] */
               r1=46u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01b36 movs r1, #0x2e */
               *(volatile u8*)((r0+6))=(u8)r1;                                                                      /* 01b38 strb r1, [r0, #6] */
               *(volatile u8*)((r0+5))=(u8)r1;                                                                      /* 01b3a strb r1, [r0, #5] */
               *(volatile u8*)((r0+4))=(u8)r1;                                                                      /* 01b3c strb r1, [r0, #4] */
               r1=82u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01b3e movs r1, #0x52 */
               *(volatile u8*)((r0+3))=(u8)r1;                                                                      /* 01b40 strb r1, [r0, #3] */
               *(volatile u8*)((r0+2))=(u8)r1;                                                                      /* 01b42 strb r1, [r0, #2] */
               r1=85u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01b44 movs r1, #0x55 */
               *(volatile u8*)((r0+1))=(u8)r1;                                                                      /* 01b46 strb r1, [r0, #1] */
               r1=80u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01b48 movs r1, #0x50 */
               goto L_08001b7c;                                                                                     /* 01b4a b #0x8001b7c */
L_08001b4c: ;  r2=r5; N=(r2)>>31; Z=(r2)==0;                                                                        /* 01b4c movs r2, r5 */
               { u32 X_=(u32)(r2), Y_=(u32)(45u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01b4e adds r2, #0x2d */
               r3=3u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 01b50 movs r3, #3 */
               goto L_08001dda;                                                                                     /* 01b52 b #0x8001dda */
L_08001b54: ;  r2=*(volatile u8*)((r0+7));                                                                          /* 01b54 ldrb r2, [r0, #7] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(3u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01b56 cmp r2, #3 */
               if((C&&!Z)) goto L_08001b5e;                                                                         /* 01b58 bhi #0x8001b5e */
L_08001b5a: ;  { u32 X_=(u32)(r2), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01b5a adds r2, r2, #1 */
               *(volatile u8*)((r0+7))=(u8)r2;                                                                      /* 01b5c strb r2, [r0, #7] */
L_08001b5e: ;  r2=60u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 01b5e movs r2, #0x3c */
               *(volatile u16*)((r0+r1))=(u16)r2;                                                                   /* 01b60 strh r2, [r0, r1] */
               r0=0x030002A4u;                                                                                      /* 01b62 ldr r0, [pc, #0x38] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 01b64 movs r1, #0 */
               *(volatile u8*)((r0+6))=(u8)r1;                                                                      /* 01b66 strb r1, [r0, #6] */
               r1=33u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01b68 movs r1, #0x21 */
               *(volatile u8*)((r0+5))=(u8)r1;                                                                      /* 01b6a strb r1, [r0, #5] */
               r1=89u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01b6c movs r1, #0x59 */
               *(volatile u8*)((r0+4))=(u8)r1;                                                                      /* 01b6e strb r1, [r0, #4] */
               r1=80u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01b70 movs r1, #0x50 */
               *(volatile u8*)((r0+3))=(u8)r1;                                                                      /* 01b72 strb r1, [r0, #3] */
               *(volatile u8*)((r0+2))=(u8)r1;                                                                      /* 01b74 strb r1, [r0, #2] */
               r1=65u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01b76 movs r1, #0x41 */
               *(volatile u8*)((r0+1))=(u8)r1;                                                                      /* 01b78 strb r1, [r0, #1] */
               r1=72u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01b7a movs r1, #0x48 */
L_08001b7c: ;  *(volatile u8*)(r0)=(u8)r1;                                                                          /* 01b7c strb r1, [r0] */
L_08001b7e: ;  r0=100u; N=(r0)>>31; Z=(r0)==0;                                                                      /* 01b7e movs r0, #0x64 */
               *(volatile u32*)(r5)=(u32)r0;                                                                        /* 01b80 str r0, [r5] */
               goto L_08001f2c;                                                                                     /* 01b82 b #0x8001f2c */
L_08001b88: ;  { u32 X_=(u32)(r4), Y_=(~(u32)(r5)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01b88 cmp r4, r5 */
               if((Z||N!=V)) goto L_08001c22;                                                                       /* 01b8a ble #0x8001c22 */
L_08001b8c: ;  { u32 X_=(u32)(r6), Y_=(u32)(20u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r6=R_; } /* 01b8c adds r6, #0x14 */
L_08001b8e: ;  { u32 X_=(u32)(r4), Y_=(~(u32)(r5)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01b8e cmp r4, r5 */
               if((N!=V)) goto L_08001c16;                                                                          /* 01b90 blt #0x8001c16 */
L_08001b92: ;  r3=0u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 01b92 movs r3, #0 */
               goto L_08001c18;                                                                                     /* 01b94 b #0x8001c18 */
L_08001be8: ;  r2=0x08010E27u;                                                                                      /* 01be8 ldr r2, [pc, #0x3c0] */
L_08001bea: ;  r1=*(volatile u8*)(r2);                                                                              /* 01bea ldrb r1, [r2] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01bec cmp r1, #0 */
               if(!Z) goto L_08001bf2;                                                                              /* 01bee bne #0x8001bf2 */
L_08001bf0: ;  goto L_08001d76;                                                                                     /* 01bf0 b #0x8001d76 */
L_08001bf2: ;  { u32 X_=(u32)(r2), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01bf2 adds r2, r2, #1 */
               r4=0u; N=(r4)>>31; Z=(r4)==0;                                                                        /* 01bf4 movs r4, #0 */
               r3=0x030002A4u;                                                                                      /* 01bf6 ldr r3, [pc, #0x374] */
L_08001bf8: ;  *(volatile u8*)((r3+r4))=(u8)r1;                                                                     /* 01bf8 strb r1, [r3, r4] */
               r1=*(volatile u8*)((r2+r4));                                                                         /* 01bfa ldrb r1, [r2, r4] */
               { u32 X_=(u32)(r4), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r5=R_; } /* 01bfc adds r5, r4, #1 */
               { u32 X_=(u32)(r1), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01bfe cmp r1, #0 */
               if(Z) goto L_08001c0a;                                                                               /* 01c00 beq #0x8001c0a */
L_08001c02: ;  { u32 X_=(u32)(r4), Y_=(~(u32)(37u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01c02 cmp r4, #0x25 */
               sp-=4; *(volatile u32*)(sp+0)=r5;                                                                    /* 01c04 push {r5} */
               r4=*(volatile u32*)(sp+0); sp+=4;                                                                    /* 01c06 pop {r4} */
               if(!C) goto L_08001bf8;                                                                              /* 01c08 blo #0x8001bf8 */
L_08001c0a: ;  { u32 X_=(u32)(r3), Y_=(u32)(r5); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01c0a adds r2, r3, r5 */
               goto L_08001d78;                                                                                     /* 01c0c b #0x8001d78 */
L_08001c0e: ;  r2=r5; N=(r2)>>31; Z=(r2)==0;                                                                        /* 01c0e movs r2, r5 */
               { u32 X_=(u32)(r2), Y_=(u32)(57u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01c10 adds r2, #0x39 */
               r3=4u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 01c12 movs r3, #4 */
               goto L_08001dda;                                                                                     /* 01c14 b #0x8001dda */
L_08001c16: ;  r3=1u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 01c16 movs r3, #1 */
L_08001c18: ;  r1=0x03000358u;                                                                                      /* 01c18 ldr r1, [pc, #0x368] */
               *(volatile u8*)(r1)=(u8)r3;                                                                          /* 01c1a strb r3, [r1] */
               { u32 X_=(u32)(r6), Y_=(u32)(r4); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r4=R_; } /* 01c1c adds r4, r6, r4 */
               r1=0x03000004u;                                                                                      /* 01c1e ldr r1, [pc, #0x354] */
               *(volatile u32*)(r1)=(u32)r4;                                                                        /* 01c20 str r4, [r1] */
L_08001c22: ;  { u32 X_=(u32)(r4), Y_=(~(u32)(r0)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01c22 cmp r4, r0 */
               if((Z||N!=V)) goto L_08001c2c;                                                                       /* 01c24 ble #0x8001c2c */
L_08001c26: ;  r1=0x03000004u;                                                                                      /* 01c26 ldr r1, [pc, #0x34c] */
               *(volatile u32*)(r1)=(u32)r0;                                                                        /* 01c28 str r0, [r1] */
               r4=r0; N=(r4)>>31; Z=(r4)==0;                                                                        /* 01c2a movs r4, r0 */
L_08001c2c: ;  r1=1u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 01c2c movs r1, #1 */
               r3=~r5; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01c2e mvns r3, r5 */
               { u32 X_=(u32)(r3), Y_=(u32)(r4); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r3=R_; } /* 01c30 adds r3, r3, r4 */
               { u32 X_=(u32)(0u), Y_=(~(u32)(r3)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r6=R_; } /* 01c32 rsbs r6, r3, #0 */
               { u32 X_=(u32)(r6), Y_=(u32)(r3); unsigned long long W_=(unsigned long long)X_+Y_+(u32)C; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r6=R_; } /* 01c34 adcs r6, r3 */
               { u32 X_=(u32)(r4), Y_=(~(u32)(r5)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01c36 cmp r4, r5 */
               sp-=4; *(volatile u32*)(sp+0)=r1;                                                                    /* 01c38 push {r1} */
               r3=*(volatile u32*)(sp+0); sp+=4;                                                                    /* 01c3a pop {r3} */
               if(Z) goto L_08001c40;                                                                               /* 01c3c beq #0x8001c40 */
L_08001c3e: ;  r3=r6; N=(r3)>>31; Z=(r3)==0;                                                                        /* 01c3e movs r3, r6 */
L_08001c40: ;  r6=~r4; N=(r6)>>31; Z=(r6)==0;                                                                       /* 01c40 mvns r6, r4 */
               { u32 X_=(u32)(r6), Y_=(u32)(r5); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r6=R_; } /* 01c42 adds r6, r6, r5 */
               { u32 X_=(u32)(0u), Y_=(~(u32)(r6)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r5=R_; } /* 01c44 rsbs r5, r6, #0 */
               { u32 X_=(u32)(r5), Y_=(u32)(r6); unsigned long long W_=(unsigned long long)X_+Y_+(u32)C; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r5=R_; } /* 01c46 adcs r5, r6 */
               { u32 X_=(u32)(r3), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01c48 cmp r3, #0 */
               if(!Z) goto L_08001c4e;                                                                              /* 01c4a bne #0x8001c4e */
L_08001c4c: ;  r1=r5; N=(r1)>>31; Z=(r1)==0;                                                                        /* 01c4c movs r1, r5 */
L_08001c4e: ;  { u32 X_=(u32)(r1), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01c4e cmp r1, #1 */
               if(Z) goto L_08001c54;                                                                               /* 01c50 beq #0x8001c54 */
L_08001c52: ;  goto L_08001f2c;                                                                                     /* 01c52 b #0x8001f2c */
L_08001c54: ;  r1=1u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 01c54 movs r1, #1 */
               r4=(u32)(r4&~(r1)); N=(r4)>>31; Z=(r4)==0;                                                           /* 01c56 bics r4, r1 */
               r1=0x03000004u;                                                                                      /* 01c58 ldr r1, [pc, #0x318] */
               *(volatile u32*)(r1)=(u32)r4;                                                                        /* 01c5a str r4, [r1] */
               r5=0x03000000u;                                                                                      /* 01c5c ldr r5, [pc, #0x2d4] */
               r1=*(volatile u32*)(r5);                                                                             /* 01c5e ldr r1, [r5] */
               { u32 X_=(u32)(r1), S_=(u32)(13u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r3=R_; N=R_>>31; Z=R_==0; } /* 01c60 lsls r3, r1, #0xd */
               r3=(u32)(r3^(r1)); N=(r3)>>31; Z=(r3)==0;                                                            /* 01c62 eors r3, r1 */
               { u32 X_=(u32)(r3), S_=(u32)(17u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r1=R_; N=R_>>31; Z=R_==0; } /* 01c64 lsrs r1, r3, #0x11 */
               r1=(u32)(r1^(r3)); N=(r1)>>31; Z=(r1)==0;                                                            /* 01c66 eors r1, r3 */
               { u32 X_=(u32)(r1), S_=(u32)(5u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r3=R_; N=R_>>31; Z=R_==0; } /* 01c68 lsls r3, r1, #5 */
               r3=(u32)(r3^(r1)); N=(r3)>>31; Z=(r3)==0;                                                            /* 01c6a eors r3, r1 */
               { u32 X_=(u32)(r3), S_=(u32)(16u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r1=R_; N=R_>>31; Z=R_==0; } /* 01c6c lsrs r1, r3, #0x10 */
               r4=150u; N=(r4)>>31; Z=(r4)==0;                                                                      /* 01c6e movs r4, #0x96 */
               r4=(u32)(r1*(r4)); N=(r4)>>31; Z=(r4)==0;                                                            /* 01c70 muls r4, r1, r4 */
               { u32 X_=(u32)(r4), S_=(u32)(16u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r1=R_; N=R_>>31; Z=R_==0; } /* 01c72 lsrs r1, r4, #0x10 */
               { u32 X_=(u32)(r1), Y_=(u32)(60u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 01c74 adds r1, #0x3c */
               *(volatile u32*)(r2)=(u32)r1;                                                                        /* 01c76 str r1, [r2] */
               { u32 X_=(u32)(r3), S_=(u32)(13u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 01c78 lsls r1, r3, #0xd */
               r1=(u32)(r1^(r3)); N=(r1)>>31; Z=(r1)==0;                                                            /* 01c7a eors r1, r3 */
               { u32 X_=(u32)(r1), S_=(u32)(17u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r2=R_; N=R_>>31; Z=R_==0; } /* 01c7c lsrs r2, r1, #0x11 */
               r2=(u32)(r2^(r1)); N=(r2)>>31; Z=(r2)==0;                                                            /* 01c7e eors r2, r1 */
               { u32 X_=(u32)(r2), S_=(u32)(5u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 01c80 lsls r1, r2, #5 */
               r1=(u32)(r1^(r2)); N=(r1)>>31; Z=(r1)==0;                                                            /* 01c82 eors r1, r2 */
               *(volatile u32*)(r5)=(u32)r1;                                                                        /* 01c84 str r1, [r5] */
               { u32 X_=(u32)(r1), S_=(u32)(16u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r1=R_; N=R_>>31; Z=R_==0; } /* 01c86 lsrs r1, r1, #0x10 */
               r2=0x01FFFFD9u;                                                                                      /* 01c88 ldr r2, [pc, #0x324] */
               { u32 X_=(u32)(r0), Y_=(u32)(r2); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 01c8a adds r0, r0, r2 */
               r0=(u32)(r1*(r0)); N=(r0)>>31; Z=(r0)==0;                                                            /* 01c8c muls r0, r1, r0 */
               { u32 X_=(u32)(r0), S_=(u32)(16u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r0=R_; N=R_>>31; Z=R_==0; } /* 01c8e lsrs r0, r0, #0x10 */
               { u32 X_=(u32)(r0), Y_=(u32)(40u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 01c90 adds r0, #0x28 */
               r1=0x00000257u;                                                                                      /* 01c92 ldr r1, [pc, #0x2c0] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(89u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 01c94 subs r1, #0x59 */
               r1=(u32)(r1&(r0)); N=(r1)>>31; Z=(r1)==0;                                                            /* 01c96 ands r1, r0 */
               r0=0x03000008u;                                                                                      /* 01c98 ldr r0, [pc, #0x2dc] */
               *(volatile u32*)(r0)=(u32)r1;                                                                        /* 01c9a str r1, [r0] */
               goto L_08001f2c;                                                                                     /* 01c9c b #0x8001f2c */
L_08001c9e: ;  r2=0x030002A4u;                                                                                      /* 01c9e ldr r2, [pc, #0x2cc] */
               r0=0u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 01ca0 movs r0, #0 */
               *(volatile u32*)((sp+32))=(u32)r0;                                                                   /* 01ca2 str r0, [sp, #0x20] */
               *(volatile u8*)((r2+17))=(u8)r0;                                                                     /* 01ca4 strb r0, [r2, #0x11] */
               r3=33u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01ca6 movs r3, #0x21 */
               *(volatile u8*)((r2+16))=(u8)r3;                                                                     /* 01ca8 strb r3, [r2, #0x10] */
               r0=r5; N=(r0)>>31; Z=(r0)==0;                                                                        /* 01caa movs r0, r5 */
               r5=83u; N=(r5)>>31; Z=(r5)==0;                                                                       /* 01cac movs r5, #0x53 */
               *(volatile u8*)((r2+15))=(u8)r5;                                                                     /* 01cae strb r5, [r2, #0xf] */
               *(volatile u8*)((r2+14))=(u8)r5;                                                                     /* 01cb0 strb r5, [r2, #0xe] */
               r4=69u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 01cb2 movs r4, #0x45 */
               *(volatile u8*)((r2+13))=(u8)r4;                                                                     /* 01cb4 strb r4, [r2, #0xd] */
               r3=76u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01cb6 movs r3, #0x4c */
               *(volatile u8*)((r2+12))=(u8)r3;                                                                     /* 01cb8 strb r3, [r2, #0xc] */
               r1=r6; N=(r1)>>31; Z=(r1)==0;                                                                        /* 01cba movs r1, r6 */
               r6=84u; N=(r6)>>31; Z=(r6)==0;                                                                       /* 01cbc movs r6, #0x54 */
               *(volatile u8*)((r2+11))=(u8)r6;                                                                     /* 01cbe strb r6, [r2, #0xb] */
               r6=79u; N=(r6)>>31; Z=(r6)==0;                                                                       /* 01cc0 movs r6, #0x4f */
               *(volatile u8*)((r2+10))=(u8)r6;                                                                     /* 01cc2 strb r6, [r2, #0xa] */
               r6=80u; N=(r6)>>31; Z=(r6)==0;                                                                       /* 01cc4 movs r6, #0x50 */
               *(volatile u8*)((r2+9))=(u8)r6;                                                                      /* 01cc6 strb r6, [r2, #9] */
               *(volatile u8*)((r2+8))=(u8)r5;                                                                      /* 01cc8 strb r5, [r2, #8] */
               r5=32u; N=(r5)>>31; Z=(r5)==0;                                                                       /* 01cca movs r5, #0x20 */
               *(volatile u8*)((r2+7))=(u8)r5;                                                                      /* 01ccc strb r5, [r2, #7] */
               r5=89u; N=(r5)>>31; Z=(r5)==0;                                                                       /* 01cce movs r5, #0x59 */
               *(volatile u8*)((r2+6))=(u8)r5;                                                                      /* 01cd0 strb r5, [r2, #6] */
               r5=68u; N=(r5)>>31; Z=(r5)==0;                                                                       /* 01cd2 movs r5, #0x44 */
               *(volatile u8*)((r2+5))=(u8)r5;                                                                      /* 01cd4 strb r5, [r2, #5] */
               r5=65u; N=(r5)>>31; Z=(r5)==0;                                                                       /* 01cd6 movs r5, #0x41 */
               *(volatile u8*)((r2+4))=(u8)r5;                                                                      /* 01cd8 strb r5, [r2, #4] */
               *(volatile u8*)((r2+3))=(u8)r4;                                                                      /* 01cda strb r4, [r2, #3] */
               r4=82u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 01cdc movs r4, #0x52 */
               *(volatile u8*)((r2+2))=(u8)r4;                                                                      /* 01cde strb r4, [r2, #2] */
               *(volatile u8*)((r2+1))=(u8)r3;                                                                      /* 01ce0 strb r3, [r2, #1] */
               *(volatile u8*)(r2)=(u8)r5;                                                                          /* 01ce2 strb r5, [r2] */
               r2=100u; N=(r2)>>31; Z=(r2)==0;                                                                      /* 01ce4 movs r2, #0x64 */
               *(volatile u32*)(r0)=(u32)r2;                                                                        /* 01ce6 str r2, [r0] */
               r0=0x03000028u;                                                                                      /* 01ce8 ldr r0, [pc, #0x264] */
               r0=*(volatile u8*)((r0+19));                                                                         /* 01cea ldrb r0, [r0, #0x13] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01cec cmp r0, #0 */
               if(!Z) goto L_08001cf2;                                                                              /* 01cee bne #0x8001cf2 */
L_08001cf0: ;  goto L_08001f2c;                                                                                     /* 01cf0 b #0x8001f2c */
L_08001cf2: ;  r6=r1; N=(r6)>>31; Z=(r6)==0;                                                                        /* 01cf2 movs r6, r1 */
               goto L_080013ca;   /* long branch */                                                                 /* 01cf4 bl #0x80013ca */
L_08001cf8: ;  r3=0x030002A4u;                                                                                      /* 01cf8 ldr r3, [pc, #0x270] */
               r0=0u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 01cfa movs r0, #0 */
               *(volatile u8*)((r3+14))=(u8)r0;                                                                     /* 01cfc strb r0, [r3, #0xe] */
               r4=33u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 01cfe movs r4, #0x21 */
               *(volatile u8*)((r3+13))=(u8)r4;                                                                     /* 01d00 strb r4, [r3, #0xd] */
               r4=75u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 01d02 movs r4, #0x4b */
               *(volatile u8*)((r3+12))=(u8)r4;                                                                     /* 01d04 strb r4, [r3, #0xc] */
               r4=67u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 01d06 movs r4, #0x43 */
               *(volatile u8*)((r3+11))=(u8)r4;                                                                     /* 01d08 strb r4, [r3, #0xb] */
               r2=73u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 01d0a movs r2, #0x49 */
               *(volatile u32*)((sp+32))=(u32)r2;                                                                   /* 01d0c str r2, [sp, #0x20] */
               *(volatile u8*)((r3+10))=(u8)r2;                                                                     /* 01d0e strb r2, [r3, #0xa] */
               r5=83u; N=(r5)>>31; Z=(r5)==0;                                                                       /* 01d10 movs r5, #0x53 */
               *(volatile u8*)((r3+9))=(u8)r5;                                                                      /* 01d12 strb r5, [r3, #9] */
               r6=32u; N=(r6)>>31; Z=(r6)==0;                                                                       /* 01d14 movs r6, #0x20 */
               *(volatile u8*)((r3+8))=(u8)r6;                                                                      /* 01d16 strb r6, [r3, #8] */
               r2=84u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 01d18 movs r2, #0x54 */
               *(volatile u8*)((r3+7))=(u8)r2;                                                                      /* 01d1a strb r2, [r3, #7] */
               r4=79u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 01d1c movs r4, #0x4f */
               *(volatile u8*)((r3+6))=(u8)r4;                                                                      /* 01d1e strb r4, [r3, #6] */
               r4=78u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 01d20 movs r4, #0x4e */
               *(volatile u8*)((r3+5))=(u8)r4;                                                                      /* 01d22 strb r4, [r3, #5] */
               *(volatile u8*)((r3+4))=(u8)r6;                                                                      /* 01d24 strb r6, [r3, #4] */
               *(volatile u8*)((r3+3))=(u8)r5;                                                                      /* 01d26 strb r5, [r3, #3] */
               r4=39u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 01d28 movs r4, #0x27 */
               *(volatile u8*)((r3+2))=(u8)r4;                                                                      /* 01d2a strb r4, [r3, #2] */
               r4=r0; N=(r4)>>31; Z=(r4)==0;                                                                        /* 01d2c movs r4, r0 */
               *(volatile u8*)((r3+1))=(u8)r2;                                                                      /* 01d2e strb r2, [r3, #1] */
               r0=*(volatile u32*)((sp+32));                                                                        /* 01d30 ldr r0, [sp, #0x20] */
               *(volatile u8*)(r3)=(u8)r0;                                                                          /* 01d32 strb r0, [r3] */
               r2=100u; N=(r2)>>31; Z=(r2)==0;                                                                      /* 01d34 movs r2, #0x64 */
               r0=0x0300001Cu;                                                                                      /* 01d36 ldr r0, [pc, #0x210] */
               *(volatile u32*)(r0)=(u32)r2;                                                                        /* 01d38 str r2, [r0] */
               r0=0x03000028u;                                                                                      /* 01d3a ldr r0, [pc, #0x214] */
               r2=*(volatile u8*)((r0+19));                                                                         /* 01d3c ldrb r2, [r0, #0x13] */
               { u32 X_=(u32)(r2), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01d3e cmp r2, #0 */
               if(Z) goto L_08001d54;                                                                               /* 01d40 beq #0x8001d54 */
L_08001d42: ;  r2=0x03000292u;                                                                                      /* 01d42 ldr r2, [pc, #0x1fc] */
               *(volatile u8*)(r2)=(u8)r4;                                                                          /* 01d44 strb r4, [r2] */
               r2=0x0800FC0Cu;                                                                                      /* 01d46 ldr r2, [pc, #0x258] */
               r3=r0; N=(r3)>>31; Z=(r3)==0;                                                                        /* 01d48 movs r3, r0 */
               r0=0x03000288u;                                                                                      /* 01d4a ldr r0, [pc, #0x1ec] */
               *(volatile u32*)(r0)=(u32)r2;                                                                        /* 01d4c str r2, [r0] */
               r0=r3; N=(r0)>>31; Z=(r0)==0;                                                                        /* 01d4e movs r0, r3 */
               r2=0x03000290u;                                                                                      /* 01d50 ldr r2, [pc, #0x1e8] */
               *(volatile u8*)(r2)=(u8)r4;                                                                          /* 01d52 strb r4, [r2] */
L_08001d54: ;  { u32 X_=(u32)(0u), Y_=(~(u32)(r1)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01d54 rsbs r2, r1, #0 */
               { u32 X_=(u32)(r2), Y_=(u32)(r1); unsigned long long W_=(unsigned long long)X_+Y_+(u32)C; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01d56 adcs r2, r1 */
               r0=*(volatile u8*)((r0+12));                                                                         /* 01d58 ldrb r0, [r0, #0xc] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 01d5a subs r1, r0, #1 */
               { u32 X_=(u32)(r0), Y_=(~(u32)(r1)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)C; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 01d5c sbcs r0, r1 */
               r0=(u32)(r0|(r2)); N=(r0)>>31; Z=(r0)==0;                                                            /* 01d5e orrs r0, r2 */
               if(Z) goto L_08001d64;                                                                               /* 01d60 beq #0x8001d64 */
L_08001d62: ;  goto L_08001f2c;                                                                                     /* 01d62 b #0x8001f2c */
L_08001d64: ;  r0=0x0300029Cu;                                                                                      /* 01d64 ldr r0, [pc, #0x1f8] */
               *(volatile u32*)(r0)=(u32)r4;                                                                        /* 01d66 str r4, [r0] */
               r0=0x03000298u;                                                                                      /* 01d68 ldr r0, [pc, #0x1f8] */
               r1=2u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 01d6a movs r1, #2 */
               *(volatile u32*)(r0)=(u32)r1;                                                                        /* 01d6c str r1, [r0] */
               r0=0x030002A0u;                                                                                      /* 01d6e ldr r0, [pc, #0x1f8] */
               *(volatile u32*)(r0)=(u32)r4;                                                                        /* 01d70 str r4, [r0] */
               goto L_08000c10;   /* long branch */                                                                 /* 01d72 bl #0x8000c10 */
L_08001d76: ;  r2=0x030002A4u;                                                                                      /* 01d76 ldr r2, [pc, #0x1f4] */
L_08001d78: ;  r1=100u; N=(r1)>>31; Z=(r1)==0;                                                                      /* 01d78 movs r1, #0x64 */
               *(volatile u32*)(r0)=(u32)r1;                                                                        /* 01d7a str r1, [r0] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 01d7c movs r1, #0 */
               *(volatile u8*)(r2)=(u8)r1;                                                                          /* 01d7e strb r1, [r2] */
               r0=0x03000028u;                                                                                      /* 01d80 ldr r0, [pc, #0x1cc] */
               r0=*(volatile u8*)((r0+19));                                                                         /* 01d82 ldrb r0, [r0, #0x13] */
               goto L_08001da0;                                                                                     /* 01d84 b #0x8001da0 */
L_08001d86: ;  r4=r0; N=(r4)>>31; Z=(r4)==0;                                                                        /* 01d86 movs r4, r0 */
               r2=0x0300029Cu;                                                                                      /* 01d88 ldr r2, [pc, #0x1d4] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 01d8a movs r1, #0 */
               *(volatile u32*)(r2)=(u32)r1;                                                                        /* 01d8c str r1, [r2] */
               r2=0x03000298u;                                                                                      /* 01d8e ldr r2, [pc, #0x1d4] */
               r3=5u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 01d90 movs r3, #5 */
               *(volatile u32*)(r2)=(u32)r3;                                                                        /* 01d92 str r3, [r2] */
L_08001d94: ;  r2=0x030002A0u;                                                                                      /* 01d94 ldr r2, [pc, #0x1d0] */
               *(volatile u32*)(r2)=(u32)r1;                                                                        /* 01d96 str r1, [r2] */
               r2=3u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 01d98 movs r2, #3 */
               r0=0x03000020u;                                                                                      /* 01d9a ldr r0, [pc, #0x1b0] */
               *(volatile u32*)(r0)=(u32)r2;                                                                        /* 01d9c str r2, [r0] */
L_08001d9e: ;  r0=*(volatile u8*)((r4+19));                                                                         /* 01d9e ldrb r0, [r4, #0x13] */
L_08001da0: ;  { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01da0 cmp r0, #0 */
               if(!Z) goto L_08001da6;                                                                              /* 01da2 bne #0x8001da6 */
L_08001da4: ;  goto L_08001f2c;                                                                                     /* 01da4 b #0x8001f2c */
L_08001da6: ;  r0=0x03000292u;                                                                                      /* 01da6 ldr r0, [pc, #0x198] */
L_08001da8: ;  *(volatile u8*)(r0)=(u8)r1;                                                                          /* 01da8 strb r1, [r0] */
               r0=0x0800FC24u;                                                                                      /* 01daa ldr r0, [pc, #0x1d4] */
L_08001dac: ;  *(volatile u32*)(r6)=(u32)r0;                                                                        /* 01dac str r0, [r6] */
               r0=0x03000290u;                                                                                      /* 01dae ldr r0, [pc, #0x18c] */
               *(volatile u8*)(r0)=(u8)r1;                                                                          /* 01db0 strb r1, [r0] */
               goto L_08001f2c;                                                                                     /* 01db2 b #0x8001f2c */
L_08001db4: ;  r2=r5; N=(r2)>>31; Z=(r2)==0;                                                                        /* 01db4 movs r2, r5 */
               { u32 X_=(u32)(r2), Y_=(u32)(69u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01db6 adds r2, #0x45 */
               r3=5u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 01db8 movs r3, #5 */
               goto L_08001dda;                                                                                     /* 01dba b #0x8001dda */
L_08001dbc: ;  r2=r5; N=(r2)>>31; Z=(r2)==0;                                                                        /* 01dbc movs r2, r5 */
               { u32 X_=(u32)(r2), Y_=(u32)(81u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01dbe adds r2, #0x51 */
               r3=6u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 01dc0 movs r3, #6 */
               goto L_08001dda;                                                                                     /* 01dc2 b #0x8001dda */
L_08001dc4: ;  r0=*(volatile u8*)((r0+13));                                                                         /* 01dc4 ldrb r0, [r0, #0xd] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01dc6 cmp r0, #0 */
               if(!Z) goto L_08001dcc;                                                                              /* 01dc8 bne #0x8001dcc */
L_08001dca: ;  goto L_08001f28;                                                                                     /* 01dca b #0x8001f28 */
L_08001dcc: ;  r0=0x08011151u;                                                                                      /* 01dcc ldr r0, [pc, #0x1d8] */
               { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&o_set_message)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* o_set_message */ /* 01dce bl #0x800e854 */
               goto L_08001f2c;                                                                                     /* 01dd2 b #0x8001f2c */
L_08001dd4: ;  r2=r5; N=(r2)>>31; Z=(r2)==0;                                                                        /* 01dd4 movs r2, r5 */
               { u32 X_=(u32)(r2), Y_=(u32)(93u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01dd6 adds r2, #0x5d */
               r3=7u; N=(r3)>>31; Z=(r3)==0;                                                                        /* 01dd8 movs r3, #7 */
L_08001dda: ;  r0=0x03000000u;                                                                                      /* 01dda ldr r0, [pc, #0x158] */
               r0=*(volatile u32*)(r0);                                                                             /* 01ddc ldr r0, [r0] */
               { u32 X_=(u32)(r0), S_=(u32)(13u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 01dde lsls r1, r0, #0xd */
               r1=(u32)(r1^(r0)); N=(r1)>>31; Z=(r1)==0;                                                            /* 01de0 eors r1, r0 */
               { u32 X_=(u32)(r1), S_=(u32)(17u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r0=R_; N=R_>>31; Z=R_==0; } /* 01de2 lsrs r0, r1, #0x11 */
               r0=(u32)(r0^(r1)); N=(r0)>>31; Z=(r0)==0;                                                            /* 01de4 eors r0, r1 */
               { u32 X_=(u32)(r0), S_=(u32)(5u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r6=R_; N=R_>>31; Z=R_==0; } /* 01de6 lsls r6, r0, #5 */
               r6=(u32)(r6^(r0)); N=(r6)>>31; Z=(r6)==0;                                                            /* 01de8 eors r6, r0 */
               { u32 X_=(u32)(r6), S_=(u32)(13u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r0=R_; N=R_>>31; Z=R_==0; } /* 01dea lsls r0, r6, #0xd */
               r0=(u32)(r0^(r6)); N=(r0)>>31; Z=(r0)==0;                                                            /* 01dec eors r0, r6 */
               { u32 X_=(u32)(r0), S_=(u32)(17u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r4=R_; N=R_>>31; Z=R_==0; } /* 01dee lsrs r4, r0, #0x11 */
               r4=(u32)(r4^(r0)); N=(r4)>>31; Z=(r4)==0;                                                            /* 01df0 eors r4, r0 */
               { u32 X_=(u32)(r4), S_=(u32)(5u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 01df2 lsls r1, r4, #5 */
               r1=(u32)(r1^(r4)); N=(r1)>>31; Z=(r1)==0;                                                            /* 01df4 eors r1, r4 */
               { u32 X_=(u32)(r1), S_=(u32)(16u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r0=R_; N=R_>>31; Z=R_==0; } /* 01df6 lsrs r0, r1, #0x10 */
               r4=208u; N=(r4)>>31; Z=(r4)==0;                                                                      /* 01df8 movs r4, #0xd0 */
               r4=(u32)(r0*(r4)); N=(r4)>>31; Z=(r4)==0;                                                            /* 01dfa muls r4, r0, r4 */
               { u32 X_=(u32)(r4), S_=(u32)(16u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r0=R_; N=R_>>31; Z=R_==0; } /* 01dfc lsrs r0, r4, #0x10 */
               { u32 X_=(u32)(r0), Y_=(u32)(16u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 01dfe adds r0, #0x10 */
               r4=12u; N=(r4)>>31; Z=(r4)==0;                                                                       /* 01e00 movs r4, #0xc */
               r4=(u32)(r3*(r4)); N=(r4)>>31; Z=(r4)==0;                                                            /* 01e02 muls r4, r3, r4 */
               *(volatile u16*)((r5+r4))=(u16)r0;                                                                   /* 01e04 strh r0, [r5, r4] */
               r0=1u; N=(r0)>>31; Z=(r0)==0;                                                                        /* 01e06 movs r0, #1 */
               *(volatile u8*)(r2)=(u8)r0;                                                                          /* 01e08 strb r0, [r2] */
               { u32 X_=(u32)(r5), Y_=(u32)(r4); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r2=R_; } /* 01e0a adds r2, r5, r4 */
               r3=15u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01e0c movs r3, #0xf */
               { u32 X_=(u32)(r3), S_=(u32)(9u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r3=R_; N=R_>>31; Z=R_==0; } /* 01e0e lsls r3, r3, #9 */
               *(volatile u32*)((r2+4))=(u32)r3;                                                                    /* 01e10 str r3, [r2, #4] */
               r3=0x4CCD0000u;                                                                                      /* 01e12 ldr r3, [pc, #0x17c] */
               { u32 X_=(u32)(r6), Y_=(~(u32)(r3)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01e14 cmp r6, r3 */
               sp-=4; *(volatile u32*)(sp+0)=r0;                                                                    /* 01e16 push {r0} */
               r3=*(volatile u32*)(sp+0); sp+=4;                                                                    /* 01e18 pop {r3} */
               if(!C) goto L_08001e1e;                                                                              /* 01e1a blo #0x8001e1e */
L_08001e1c: ;  r3=*(volatile u32*)((sp+20));                                                                        /* 01e1c ldr r3, [sp, #0x14] */
L_08001e1e: ;  r4=0x11EC0000u;                                                                                      /* 01e1e ldr r4, [pc, #0x174] */
               { u32 X_=(u32)(r6), Y_=(~(u32)(r4)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01e20 cmp r6, r4 */
               r5=*(volatile u32*)((sp+28));                                                                        /* 01e22 ldr r5, [sp, #0x1c] */
               if(!C) goto L_08001e28;                                                                              /* 01e24 blo #0x8001e28 */
L_08001e26: ;  { u32 X_=(u32)(r3), S_=(u32)(1u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r0=R_; N=R_>>31; Z=R_==0; } /* 01e26 lsls r0, r3, #1 */
L_08001e28: ;  *(volatile u8*)((r2+8))=(u8)r0;                                                                      /* 01e28 strb r0, [r2, #8] */
               { u32 X_=(u32)(r1), S_=(u32)(13u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r0=R_; N=R_>>31; Z=R_==0; } /* 01e2a lsls r0, r1, #0xd */
               r0=(u32)(r0^(r1)); N=(r0)>>31; Z=(r0)==0;                                                            /* 01e2c eors r0, r1 */
               { u32 X_=(u32)(r0), S_=(u32)(17u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r1=R_; N=R_>>31; Z=R_==0; } /* 01e2e lsrs r1, r0, #0x11 */
               r1=(u32)(r1^(r0)); N=(r1)>>31; Z=(r1)==0;                                                            /* 01e30 eors r1, r0 */
               { u32 X_=(u32)(r1), S_=(u32)(5u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r0=R_; N=R_>>31; Z=R_==0; } /* 01e32 lsls r0, r1, #5 */
               r0=(u32)(r0^(r1)); N=(r0)>>31; Z=(r0)==0;                                                            /* 01e34 eors r0, r1 */
               { u32 X_=(u32)(r0), S_=(u32)(16u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r1=R_; N=R_>>31; Z=R_==0; } /* 01e36 lsrs r1, r0, #0x10 */
               r3=160u; N=(r3)>>31; Z=(r3)==0;                                                                      /* 01e38 movs r3, #0xa0 */
               r3=(u32)(r1*(r3)); N=(r3)>>31; Z=(r3)==0;                                                            /* 01e3a muls r3, r1, r3 */
               { u32 X_=(u32)(r3), S_=(u32)(16u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r1=R_; N=R_>>31; Z=R_==0; } /* 01e3c lsrs r1, r3, #0x10 */
               r3=14u; N=(r3)>>31; Z=(r3)==0;                                                                       /* 01e3e movs r3, #0xe */
               r3=(u32)(r5*(r3)); N=(r3)>>31; Z=(r3)==0;                                                            /* 01e40 muls r3, r5, r3 */
               { u32 X_=(u32)(r3), Y_=(u32)(r1); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 01e42 adds r1, r3, r1 */
               { u32 X_=(u32)(r1), Y_=(u32)(200u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 01e44 adds r1, #0xc8 */
               *(volatile u16*)((r2+2))=(u16)r1;                                                                    /* 01e46 strh r1, [r2, #2] */
               r4=0x03000288u;                                                                                      /* 01e48 ldr r4, [pc, #0xec] */
               r6=0x03000292u;                                                                                      /* 01e4a ldr r6, [pc, #0xf4] */
               r2=0x03000348u;                                                                                      /* 01e4c ldr r2, [pc, #0x13c] */
L_08001e4e: ;  { u32 X_=(u32)(r0), S_=(u32)(13u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 01e4e lsls r1, r0, #0xd */
               r1=(u32)(r1^(r0)); N=(r1)>>31; Z=(r1)==0;                                                            /* 01e50 eors r1, r0 */
               { u32 X_=(u32)(r1), S_=(u32)(17u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r0=R_; N=R_>>31; Z=R_==0; } /* 01e52 lsrs r0, r1, #0x11 */
               r0=(u32)(r0^(r1)); N=(r0)>>31; Z=(r0)==0;                                                            /* 01e54 eors r0, r1 */
               { u32 X_=(u32)(r0), S_=(u32)(5u)&0xFF; u32 R_; if(S_==0){R_=X_;} else {C=(X_>>(32-S_))&1; R_=X_<<S_;} r1=R_; N=R_>>31; Z=R_==0; } /* 01e56 lsls r1, r0, #5 */
               r1=(u32)(r1^(r0)); N=(r1)>>31; Z=(r1)==0;                                                            /* 01e58 eors r1, r0 */
               r0=0x03000000u;                                                                                      /* 01e5a ldr r0, [pc, #0xd8] */
               *(volatile u32*)(r0)=(u32)r1;                                                                        /* 01e5c str r1, [r0] */
               { u32 X_=(u32)(r1), S_=(u32)(16u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r0=R_; N=R_>>31; Z=R_==0; } /* 01e5e lsrs r0, r1, #0x10 */
               r1=24u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01e60 movs r1, #0x18 */
               r1=(u32)(r0*(r1)); N=(r1)>>31; Z=(r1)==0;                                                            /* 01e62 muls r1, r0, r1 */
               { u32 X_=(u32)(r1), S_=(u32)(16u)&0xFF; u32 R_; if(S_==0) S_=32; if(S_<32){C=(X_>>(S_-1))&1; R_=X_>>S_;} else {C=X_>>31; R_=0;} r0=R_; N=R_>>31; Z=R_==0; } /* 01e64 lsrs r0, r1, #0x10 */
               { u32 X_=(u32)(r0), Y_=(~(u32)(r5)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 01e66 subs r0, r0, r5 */
               r1=21u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01e68 movs r1, #0x15 */
               r1=~r1; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01e6a mvns r1, r1 */
               { u32 X_=(u32)(r0), Y_=(~(u32)(r1)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01e6c cmp r0, r1 */
               if((!Z&&N==V)) goto L_08001e72;                                                                      /* 01e6e bgt #0x8001e72 */
L_08001e70: ;  r0=r1; N=(r0)>>31; Z=(r0)==0;                                                                        /* 01e70 movs r0, r1 */
L_08001e72: ;  { u32 X_=(u32)(r0), Y_=(u32)(34u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 01e72 adds r0, #0x22 */
               *(volatile u32*)(r2)=(u32)r0;                                                                        /* 01e74 str r0, [r2] */
L_08001e76: ;  r0=0x03000344u;                                                                                      /* 01e76 ldr r0, [pc, #0x120] */
               r1=*(volatile u32*)(r0);                                                                             /* 01e78 ldr r1, [r0] */
               { u32 X_=(u32)(r1), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 01e7a subs r1, r1, #1 */
               *(volatile u32*)(r0)=(u32)r1;                                                                        /* 01e7c str r1, [r0] */
               if((!Z&&N==V)) goto L_08001f2c;                                                                      /* 01e7e bgt #0x8001f2c */
L_08001e80: ;  r0=0x030002CCu;                                                                                      /* 01e80 ldr r0, [pc, #0xec] */
               r1=0u; N=(r1)>>31; Z=(r1)==0;                                                                        /* 01e82 movs r1, #0 */
               *(volatile u32*)(r0)=(u32)r1;                                                                        /* 01e84 str r1, [r0] */
               r2=2u; N=(r2)>>31; Z=(r2)==0;                                                                        /* 01e86 movs r2, #2 */
               r0=0x0300034Cu;                                                                                      /* 01e88 ldr r0, [pc, #0xf0] */
               *(volatile u32*)(r0)=(u32)r2;                                                                        /* 01e8a str r2, [r0] */
               r5=0x03000028u;                                                                                      /* 01e8c ldr r5, [pc, #0xc0] */
               r0=*(volatile u8*)((r5+20));                                                                         /* 01e8e ldrb r0, [r5, #0x14] */
               r3=*(volatile u32*)((sp+32));                                                                        /* 01e90 ldr r3, [sp, #0x20] */
               { u32 X_=(u32)(r3), Y_=(~(u32)(r0)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01e92 cmp r3, r0 */
               if((Z||N!=V)) goto L_08001ea0;                                                                       /* 01e94 ble #0x8001ea0 */
L_08001e96: ;  r0=*(volatile u32*)((sp+32));                                                                        /* 01e96 ldr r0, [sp, #0x20] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(250u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01e98 cmp r0, #0xfa */
               if((N!=V)) goto L_08001e9e;                                                                          /* 01e9a blt #0x8001e9e */
L_08001e9c: ;  r0=250u; N=(r0)>>31; Z=(r0)==0;                                                                      /* 01e9c movs r0, #0xfa */
L_08001e9e: ;  *(volatile u8*)((r5+20))=(u8)r0;                                                                     /* 01e9e strb r0, [r5, #0x14] */
L_08001ea0: ;  r0=*(volatile u8*)((r5+4));                                                                          /* 01ea0 ldrb r0, [r5, #4] */
               r3=0x0800FE20u;                                                                                      /* 01ea2 ldr r3, [pc, #0xf8] */
               r3=*(volatile u8*)((r3+r0));                                                                         /* 01ea4 ldrb r3, [r3, r0] */
               r0=*(volatile u8*)((r5+9));                                                                          /* 01ea6 ldrb r0, [r5, #9] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(r3)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01ea8 cmp r0, r3 */
               if((!C||Z)) goto L_08001eb0;                                                                         /* 01eaa bls #0x8001eb0 */
L_08001eac: ;  { u32 X_=(u32)(r0), Y_=(~(u32)(1u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 01eac subs r0, r0, #1 */
               *(volatile u8*)((r5+9))=(u8)r0;                                                                      /* 01eae strb r0, [r5, #9] */
L_08001eb0: ;  r0=*(volatile u32*)((sp+32));                                                                        /* 01eb0 ldr r0, [sp, #0x20] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(12u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01eb2 cmp r0, #0xc */
               if((N!=V)) goto L_08001ed6;                                                                          /* 01eb4 blt #0x8001ed6 */
L_08001eb6: ;  r0=*(volatile u8*)((r5+7));                                                                          /* 01eb6 ldrb r0, [r5, #7] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(2u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01eb8 cmp r0, #2 */
               if(!C) goto L_08001ebe;                                                                              /* 01eba blo #0x8001ebe */
L_08001ebc: ;  r0=r2; N=(r0)>>31; Z=(r0)==0;                                                                        /* 01ebc movs r0, r2 */
L_08001ebe: ;  { u32 X_=(u32)(r0), Y_=(u32)(2u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 01ebe adds r0, r0, #2 */
               *(volatile u8*)((r5+7))=(u8)r0;                                                                      /* 01ec0 strb r0, [r5, #7] */
               r0=*(volatile u8*)((r5+8));                                                                          /* 01ec2 ldrb r0, [r5, #8] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(3u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01ec4 cmp r0, #3 */
               if((C&&!Z)) goto L_08001ecc;                                                                         /* 01ec6 bhi #0x8001ecc */
L_08001ec8: ;  { u32 X_=(u32)(r0), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 01ec8 adds r0, r0, #1 */
               *(volatile u8*)((r5+8))=(u8)r0;                                                                      /* 01eca strb r0, [r5, #8] */
L_08001ecc: ;  r0=*(volatile u32*)((sp+24));                                                                        /* 01ecc ldr r0, [sp, #0x18] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01ece cmp r0, #0 */
               if(Z) goto L_08001f0a;                                                                               /* 01ed0 beq #0x8001f0a */
L_08001ed2: ;  r0=0x0800FE9Cu;                                                                                      /* 01ed2 ldr r0, [pc, #0xd0] */
               goto L_08001eec;                                                                                     /* 01ed4 b #0x8001eec */
L_08001ed6: ;  { u32 X_=(u32)(r0), Y_=(~(u32)(6u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01ed6 cmp r0, #6 */
               if((N!=V)) goto L_08001efa;                                                                          /* 01ed8 blt #0x8001efa */
L_08001eda: ;  r0=*(volatile u8*)((r5+7));                                                                          /* 01eda ldrb r0, [r5, #7] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(3u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01edc cmp r0, #3 */
               if((C&&!Z)) goto L_08001ee4;                                                                         /* 01ede bhi #0x8001ee4 */
L_08001ee0: ;  { u32 X_=(u32)(r0), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 01ee0 adds r0, r0, #1 */
               *(volatile u8*)((r5+7))=(u8)r0;                                                                      /* 01ee2 strb r0, [r5, #7] */
L_08001ee4: ;  r0=*(volatile u32*)((sp+24));                                                                        /* 01ee4 ldr r0, [sp, #0x18] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01ee6 cmp r0, #0 */
               if(Z) goto L_08001f0a;                                                                               /* 01ee8 beq #0x8001f0a */
L_08001eea: ;  r0=0x0800FE84u;                                                                                      /* 01eea ldr r0, [pc, #0x9c] */
L_08001eec: ;  r2=0x0300028Cu;                                                                                      /* 01eec ldr r2, [pc, #0x54] */
               *(volatile u32*)(r2)=(u32)r0;                                                                        /* 01eee str r0, [r2] */
               r0=0x03000290u;                                                                                      /* 01ef0 ldr r0, [pc, #0x48] */
               { u32 X_=(u32)(r0), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 01ef2 adds r0, r0, #1 */
               r2=0x03000292u;                                                                                      /* 01ef4 ldr r2, [pc, #0x48] */
               { u32 X_=(u32)(r2), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r6=R_; } /* 01ef6 adds r6, r2, #1 */
               goto L_08001f06;                                                                                     /* 01ef8 b #0x8001f06 */
L_08001efa: ;  r0=*(volatile u32*)((sp+24));                                                                        /* 01efa ldr r0, [sp, #0x18] */
               { u32 X_=(u32)(r0), Y_=(~(u32)(0u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01efc cmp r0, #0 */
               if(Z) goto L_08001f0a;                                                                               /* 01efe beq #0x8001f0a */
L_08001f00: ;  r0=0x0800FC0Cu;                                                                                      /* 01f00 ldr r0, [pc, #0x9c] */
               *(volatile u32*)(r4)=(u32)r0;                                                                        /* 01f02 str r0, [r4] */
               r0=0x03000290u;                                                                                      /* 01f04 ldr r0, [pc, #0x34] */
L_08001f06: ;  *(volatile u8*)(r0)=(u8)r1;                                                                          /* 01f06 strb r1, [r0] */
               *(volatile u8*)(r6)=(u8)r1;                                                                          /* 01f08 strb r1, [r6] */
L_08001f0a: ;  r0=0x31504B43u;                                                                                      /* 01f0a ldr r0, [pc, #0x4c] */
               *(volatile u32*)(r5)=(u32)r0;                                                                        /* 01f0c str r0, [r5] */
               r0=0x00005EEDu;                                                                                      /* 01f0e ldr r0, [pc, #0x4c] */
               r2=31u; N=(r2)>>31; Z=(r2)==0;                                                                       /* 01f10 movs r2, #0x1f */
L_08001f12: ;  r3=*(volatile u8*)((r5+r1));                                                                         /* 01f12 ldrb r3, [r5, r1] */
               r0=(u32)(r2*(r0)); N=(r0)>>31; Z=(r0)==0;                                                            /* 01f14 muls r0, r2, r0 */
               { u32 X_=(u32)(r0), Y_=(u32)(r3); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r0=R_; } /* 01f16 adds r0, r0, r3 */
               { u32 X_=(u32)(r1), Y_=(u32)(1u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; r1=R_; } /* 01f18 adds r1, r1, #1 */
               { u32 X_=(u32)(r1), Y_=(~(u32)(72u)); unsigned long long W_=(unsigned long long)X_+Y_+(u32)1; u32 R_=(u32)W_; N=R_>>31; Z=R_==0; C=(u32)(W_>>32); V=((~(X_^Y_))&(X_^R_))>>31; } /* 01f1a cmp r1, #0x48 */
               if(!Z) goto L_08001f12;                                                                              /* 01f1c bne #0x8001f12 */
L_08001f1e: ;  *(volatile u32*)((r5+72))=(u32)r0;                                                                   /* 01f1e str r0, [r5, #0x48] */
               r1=76u; N=(r1)>>31; Z=(r1)==0;                                                                       /* 01f20 movs r1, #0x4c */
               r0=r5; N=(r0)>>31; Z=(r0)==0;                                                                        /* 01f22 movs r0, r5 */
               goto L_08000c8e;   /* long branch */                                                                 /* 01f24 bl #0x8000c8e */
L_08001f28: ;  { unsigned long long rv=((unsigned long long(*)(u32,u32,u32,u32,u32,u32))&o_minigame_start)(r0,r1,r2,r3,*(volatile u32*)sp,*(volatile u32*)(sp+4)); r0=(u32)rv; r1=(u32)(rv>>32); }   /* o_minigame_start */ /* 01f28 bl #0x800e8f0 */
L_08001f2c: ;  { u32 X_=(u32)(sp), Y_=(u32)(116u); unsigned long long W_=(unsigned long long)X_+Y_+(u32)0; u32 R_=(u32)W_; sp=R_; } /* 01f2c add sp, #0x74 */
               r4=*(volatile u32*)(sp+0); r5=*(volatile u32*)(sp+4); r6=*(volatile u32*)(sp+8); r7=*(volatile u32*)(sp+12); sp+=16; /* 01f2e pop {r4, r5, r6, r7} */
               r0=*(volatile u32*)(sp+0); sp+=4;                                                                    /* 01f30 pop {r0} */
               return;                                                                                              /* 01f32 bx r0 */
}
