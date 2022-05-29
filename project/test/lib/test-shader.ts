import { expect } from "chai";
import type { SpglslAngleCompileOptions } from "spglsl";
import { spglslAngleCompile } from "spglsl";
import chalk from "chalk";
import zlib from "node:zlib";

export interface TestShader {
  name: string;
  sourceCode: string;
  shaderType?: "vert" | "frag";
  hasIncludes?: boolean;
}

const fragmentShaderCode =
  "#version 300 es\nprecision highp float;in vec2 FC;uniform vec2 iR;uniform vec3 iP;uniform vec4 iD;uniform vec4 iA;uniform vec4 iB;uniform vec4 iC;uniform vec4 iS;uniform vec4 iX;uniform mat3 iM;uniform lowp int iF;out vec4 oC;uniform highp sampler2D tN;uniform highp sampler2D tH;uniform highp sampler2D tP;uniform highp sampler2D tS;float a;float b(float a){return clamp(a,0.,1.);}float c(float a,float b,float c){float d=max(c-abs(a-b),0.)/c;return min(a,b)-d*d*c/4.;}vec4 d(float a){vec4 c=b(a)*vec4(.999998,254.99948,65024.867,160581056);c=fract(c);c-=c.yzww*vec4(.003921569,.003921569,.003921569,0);return c;}float e(vec4 a){return dot(a,vec4(1,.003921569,1.53787e-5,6.2273724e-9));}vec3 f(vec2 a){vec4 b=texelFetch(tN,ivec2(floor(a))&511,0);float c=b.y-b.x,d=b.z-b.x,e=b.w-c-b.z;vec2 f=fract(a),g=f*f,h=g*(3.-2.*f);return vec3(b.x+c*h.x+d*h.y+e*h.x*h.y,30.*g*(f*(f-2.)+1.)*(vec2(c,d)+e*h.yx));}int g=0;float h=150.;void i(int b,float c){if(c<a&&c!=h){g=b;h=c;}}float j(vec3 a,float b){return length(a)-b;}float k(vec3 a,vec3 b){vec3 c=abs(a)-b;return min(max(c.x,max(c.y,c.z)),0.)+length(max(c,0.));}float l(vec3 a,float b){vec3 c=abs(a)-b;return min(max(c.x,max(c.y,c.z)),0.)+length(max(c,0.));}float m(vec3 a,float b,float c){float d=length(a.xy)-b;return max(d,abs(a.z)-c);}float n(vec3 a,vec2 b){return length(vec2(length(a.xz)-b.x,a.y))-b.y;}float o(float a,float b,float c,float d){float e=b*.5,f=floor((a+e)/b),g=mod(a+e,b)-e;return f>d?g+b*(f-d):f<c?g+b*(f-c):g;}vec2 p(vec2 a,float b){float c=3.1415927/b,d=mod(atan(a.y,a.x)+c,c*2.)-c;return vec2(cos(d),sin(d))*length(a);}vec2 q(vec2 a,float b,float c){float d=3.1415927/b,e=mod(atan(a.y,a.x)+d+c,d*2.)-d;return vec2(cos(e),sin(e))*length(a);}float r(float a,float b){return abs(a)-b;}mat2 s(float a){float b=cos(a),c=sin(a);return mat2(b,c,-c,b);}vec3 t(vec3 a){return vec3(a.xy,-a.z);}float u(vec3 a){float b=length(a)-.3;if(b>.3)return b;a.xz*=s(-1.2);a.yz*=s(-.2);return min(m(a,.025,.1),max(j(a-vec3(0,0,.12),.05),a.z-.12));}float v(vec3 a){float b=length(a)-.3;if(b>.3)return b;float c=m(a,.01,.06);c=min(c,m(a.yzx+vec3(0,.1,0),.04,.005));c=min(c,k(a-vec3(0,-.01,.04),vec3(.002,.02,.02)));return c;}float w(vec3 a){float b=k(a-vec3(.03,0,0),vec3(.03,.006,.03));i(1,b);return min(k(a,vec3(.06,.005,.06)),b);}float x(vec3 a,float b,float c){float d=length(a)-b*.6;if(d>4.)return d;a.y+=cos(a.z*c/b);a.x=abs(a.x);float e=k(a-vec3(.2,0,0),vec3(.1,.03,b*.55)),f=m(a-vec3(.5,1,0),.02,b*.55);a.z=o(a.z,.55,-b,b);f=min(f,m(a.xzy-vec3(.5,0,.5),.02,.5));i(1,f);e=min(e,k(a,vec3(.5,.05,.2)));i(-1,e);return min(e,f);}float y(vec3 a){float b=length(a)-2.;if(b>1.)return b;vec3 c=a;c.xy*=s(-.25);float d=k(c+vec3(.2,.25,0),vec3(.25,.5,.5))-.01;c-=vec3(-.13,.25,0);c.z=o(c.z,.04,-10.,10.);c.x=o(c.x,.04,-3.,3.);float e=l(c,.01)-.005;i(1,e);d=min(min(d,e),k(a-vec3(-.45,.2,0),vec3(.2,.8,.5))-.01);return d;}float z(vec3 a){a.zy*=s(.06);a.y+=cos(a.z/20.)*3.;return m(a,.01,27.5);}float A(vec3 a){float b=length(a)-3.;if(b>.5)return b;a.xz-=vec2(1,-.05);a.zx*=s(iA.y*-2.5);a.xz+=vec2(1,-.05);float c=m(a,.99,.05);vec3 d=vec3(p(a.xy,8.),a.z);return max(c,-min(k(d-vec3(.5,0,.1),vec3(.02,.1,.1)),m(d-vec3(0,0,.1),.02,.1)));}float B(vec3 a,float b){float c=length(a)-1.;if(c>1.)return c;float d=k(a,vec3(.2,.5,.05));d=max(d,-k(a,vec3(.03,.2,1)));a.yz*=s(-1.5707964*b+.7853982);a.z+=.2;d=min(d,m(a,.02,.2));a.z+=.2;d=min(d,m(a,.03,.05));return d;}float C(vec3 a){float b=length(a)-18.;if(b>5.)return b;a.y-=9.;vec3 c=a;c.xz*=s(iB.y);c.xy*=s(.5);c.y-=9.;float d=j(c,9.),e=max(r(d,.01),c.y+4.5);e=min(e,m(c.xzy+vec3(0,0,4.5),.1,4.5));e=min(e,j(c,.3));a.y+=6.75;float f=k(a,vec3(2.25,3.6,4.5));f=min(f,min(max(r(m(a.xzy-vec3(2.25,0,0),4.4,3.5),.1),-min(m(a.zyx-vec3(0,1.8,0),1.,100.),m(a-vec3(4.5,2.3,0),.4,100.))),m(a.xzy-vec3(2.25,0,-2.2),4.4,2.9)));float g=y(a-vec3(3,1.5,2)),h=A(a.zyx-vec3(0,1.8,6.5)),l=B(t(a-vec3(3.7,2,-4)),clamp(iA.w,0.,1.));a.y-=2.25;f=max(min(f,m(a.xzy,.45000002,4.7699995)),-d);a-=vec3(7,-2.85,0);a.xy*=s(-.5);f=min(f,k(a,vec3(1,1,.8))-.01);float n=min(e,h);i(2,l);i(3,g);i(5,n);return min(min(g,f),min(n,l));}float D(vec3 a){float b=length(a.xz)-2.;if(b>3.)return b;float c=m(a.xzy,.2,.5),d=a.y+iA.z*4.;if((iF&8)!=0){float b=v(vec3(a.x,d,a.z)+vec3(1.05,-5.05,1.05));if(b<c){i(2,b);c=b;}}float e=k(vec3(p(a.xz,8.),d).xzy-vec3(1.5,0,0),vec3(.1,5,.2));if(e<c){i(4,e);c=e;}float f=m(a.xzy+vec3(0,0,clamp(iA.z,0.,.02)),.05,.53);if(f<c){i(1,f);c=f;}return c;}float E(vec3 a){vec3 b=a.zyx-vec3(11,1.25,-44);float c=length(b)-8.;if(c>5.)return c;b.y-=2.;float d=k(b-vec3(-2.7,-1,-1.3),vec3(.35,.5,.5)),e=max(r(k(b,vec3(4,1.6,2)),.23),-min(m(b-vec3(0,.5,0),.8,100.),k(b-vec3(4,-.37,1),vec3(2,1,.53))));vec3 f=b-vec3(4,-.77,.5);f.xz*=s(-iA.x*3.1415927/2.);float g=k(f-vec3(0,.4,.5),vec3(.05,.99,.52));b.x=o(b.x,.3,-10.,10.);b.z=abs(b.z);float h=m(b.xzy-vec3(0,2,.5),.01,1.),j=min(d,g);i(1,h);i(-1,j);float l=min(h,min(e,j)),n=150.;if((iF&4)!=0)n=u(a-vec3(-42,3,11.2));if((iF&2)!=0)n=min(n,v(a.yzx-vec3(2,7.4,-45.5)));if(n<l){i(2,n);return n;}return l;}float F(vec3 a){float b=length(a)-9.;if(b>1.)return b;a.xz*=s(-.7853982);float d=k(a-vec3(-1.5,1,5),vec3(1,.2,3));a.y-=iB.w;vec3 e=a.xzy-vec3(-2,0,2);float f=c(j(a-clamp(a,vec3(-6,0,0),vec3(6,0,0)),1.7),min(m(e-clamp(e,vec3(-.5,0,0),vec3(.5,0,0)),.4,.5),min(k(a-vec3(7.5,0,0),vec3(.3,2,.05))-.05,k(a-vec3(7.5,0,0),vec3(.3,.05,2))-.05)),.3);i(3,f);return min(d,f);}float G(vec3 a){float b=length(a)-13.;if(b>3.)return b;vec3 c=abs(a),d=vec3(c.x,abs(a.y-4.58),c.z);float e=B(t(a.xzy-vec3(1.9,-1.5,.2))*.5,min(1.,(6.-iX.x)*.2))/.5,f=max(k(vec3(a.x,abs(a.y-3.5)-3.5,a.z),vec3(6,.2,6))-.05,max(-l(a-vec3(2,7,2),1.5),-l(a-vec3(5.7,0,4),.52)));vec3 g=a-vec3(5,7.6,-2);g.xy*=s(.3);vec3 h=vec3(a.xy,abs(a.z+2.));e=min(e,min(min(m(h.xzy-vec3(-6,1.1,8.7),1.,1.75),m(h.xzy-vec3(-6.5,1.1,0),.2,8.)),m(vec3(a.z,abs(a.y-7.6),a.x)-vec3(-3,.2,0),.1,5.)));float j=min(min(min(m(vec3(c.xz,a.y)-vec3(5,5,0),.5,8.3),m(d.zyx-vec3(5.3,3.5,0),.05,5.3)),max(m(d-vec3(5.3,3.5,0),.05,5.3),-l(a-vec3(5,.7,4),.8))),k(g,vec3(.5,.6,1.5))-.05);i(1,j);vec3 o=a-vec3(2,3.59,-.1);o.zy*=s(-.7853982);o.y-=iX.x;e=min(e,k(o,vec3(1,5.1,.02))-.05);i(5,e);float p=min(min(f,e),j);vec3 r=g-vec3(0,.8,0);if(length(r)-1.<2.){float a=min(min(n(r,vec2(.5,.02)),m(r.xzy+vec3(0,0,.5),.02,.5)),m(vec3(q(r.xz,5.,iB.x),r.y).zyx-vec3(0,0,.25),.01,.25));if(a<p){i(2,a);p=a;}}return p;}float H(vec3 a){vec3 b=a.zyx-vec3(4,-1,17);b.zy*=s(-.19);b.z-=19.-iA.w;return min(x(b,21.,0.),m(b.xzy+vec3(0,10.5,6),.15,5.));}float I(vec3 a){vec3 b=a-vec3(8.7,9.3,37);float c=length(b.xz)-5.;if(c>4.)return c;vec3 d=vec3(p(b.xz,6.),b.y).xzy,e=vec3(d.x,o(b.y,1.5,-3.,7.),d.z);float f=max(max(min(m(b.xzy,1.1,12.),max(r(m(b.xzy-vec3(0,0,14),4.,2.),.2),-k(d-vec3(4,14,0),vec3(1,1,2)))),-min(m(b.xzy,1.,13.),k(e-vec3(1,0,0),vec3(.2,.3,.13)))),-k(b+vec3(0,7,1),vec3(.8,1.2,.8)));vec3 g=vec3(b.x,b.y-iB.z,b.z);float h=m(g.xzy,1.,11.);g.y=o(g.y,1.5,-7.,7.);h=max(h,-n(g,vec2(1,.01)));vec3 l=vec3(b.x-.8,o(b.y-12.7,20.5,-1.,0.),b.z+.9);float q=j(l-vec3(0,.5,0),.06),s=min(m(l.xzy,.05,.5),min(k(l-vec3(0,.5,0),vec3(.05,.1,.1)),q));i(2,q);i(1,s);i(5,h);float t=min(min(f,min(s,h)),k(b+vec3(0,10.3,3),vec3(1.1,2,3)));if((iF&16)!=0){float b=w(a-vec3(12.15,22.31,38.65));if(b<t){i(2,b);return b;}}return t;}vec2 J;float K(vec3 a,vec3 b,vec2 c){a-=b;float d=length(a)-2.;if(d>.5)return d;a.xz*=s(1.5707964);J=(c-a.xy)/(c*2.);float e=k(a,vec3(c.xy,.01));return e;}float L(vec3 a){vec3 b=abs(vec3(a.x,a.y+3.,a.z))-vec3(60,0,39);if(b.x<0.&&b.z<0.)b.y-=e(textureLod(tH,a.xz/vec2(120,78)+.5,0.))*19.;return min(b.y,0.)+length(max(b,0.));}float M(vec3 a){float b=x(a-vec3(45,1.7,22.4),10.,2.),c=C(a-vec3(2,10,2)),d=D(a-vec3(47.5,3.5,30.5)),e=E(a);vec3 f=a-vec3(26,5,-58);f.xz*=s(1.9707963);float g=G(f),h=H(f),j=z(f.zyx-vec3(-2,9.7,32.5)),k=I(a),l=F(a-vec3(-46,-.5,-30)),m=min(min(min(b,c),min(d,e)),min(g,min(h,min(k,l))));if(j<m){i(1,j);return j;}i(0,m);return m;}int N=0;float O(vec3 b){float c=L(b);if(c<=a){N=1;return c;}float d=M(b),e=K(b,vec3(4.76,14.42,4),vec2(.45,.29)),f=min(e,d);if(c<f){N=1;return c;}N=e<=d?3:2;return f;}vec3 P(vec3 a){return normalize(vec3(M(a+vec3(.001,0,0)),M(a+vec3(0,.001,0)),M(a+vec3(0,0,.001)))-M(a));}vec3 Q(vec3 a,float b){vec2 c=vec2(mix(.03,.001,min(b/120.,1.)),0);return normalize(vec3(L(a+c.xyy),L(a+c.yxy),L(a+c.yyx))-L(a));}float R(vec3 b,vec3 c,float d,float e){float f=d;for(int g=0;;g++){vec3 h=b+c*e;a=d*max(e,1.);if(h.y<=-2.9995||e>=150.){float a=(-3.-b.y)/c.y;if(a>=0.&&a<500.){N=1;return a;}break;}if(h.y>45.)break;float i=O(h);if(i<0.){e-=f;i=f/2.;}e+=i;if(i<=a||g>=100)return e;f=i;}N=0;return 500.;}float S(vec3 b,float c,vec3 d,float e){float f=clamp(c*.005,.01,.1);b=b+d*f;float g=iS.w-a*2.;for(float h=1.;h<50.;h++){vec3 c=b+iS.xyz*f;if(f>=80.||c.y>45.||c.y<g||length(b)>=150.)break;float d=M(c),i=max(a,.01*min(1.,f)+h*2e-4);if(d<=i)return 0.;e=min(e,85.*d/f);if(e<.078)return 0.;f+=d+a;}return e;}float T(vec3 a,vec3 b){float c=(iS.w-a.y)/b.y;return min(c>=0.?c:500.,500.);}vec3 U(vec2 a){vec3 b=vec3(0);float c=0.,d=1.,e=0.,g=1.-length(iP.xz-a)/500.,h=5.*g*g;for(float i=0.;i<h;++i){a+=iD.w*.5;e*=-.75;vec3 g=f(a+sin(a.yx*.5+iD.w*.5)*.5);b+=g*d;a+=g.yz*.43;a*=2.;c+=d;d*=.75;}return b/c;}vec3 V(vec3 a,float c,vec3 d){float e=min(c/500.,1.),f=b(pow(e,3.5)+1.-exp(-c*.005)),g=max(dot(d,iS.xyz),0.);vec3 h=mix(vec3(.4,.8,1),vec3(1.065,.95,.85),pow(g,10.));return mix(a,h,f);}vec3 W(vec3 a,vec3 c){vec4 d=texelFetch(tP,ivec2(FC*256./iR),0);float e=uintBitsToFloat(uint(d.x*255.)<<24|uint(d.y*255.)<<16|uint(d.z*255.)<<8|uint(d.w*255.)),f=R(a,c,.001,e),h=T(a,c);vec3 i,j=vec3(0,1,0);float k=f;if(N==3)return iB.y>0.?texture(tS,J).xyz:vec3(0);vec3 l=a+c*f;bool m=h<500.&&h<f;vec3 n;float o=0.;if(m){o=mix(.2,1.,b((f-h)/3.));vec3 d=a+c*h,e=mix(vec3(0),U(d.xz*(.7-iS.w*.02))*(1.-length(d)/450.),o);j=normalize(vec3(e.y,1.,e.x));h-=abs(e.z)*o*.6;k=h;n=mix(vec3(.25,.52,.73),vec3(.15,.62,.83),b(abs(e.z)-o));}int p=N,q=g;if(N==0)i=vec3(.4,.8,1);else{vec3 a;if(l.y<=-2.9995){a=vec3(0,1,0);i=vec3(1);}else{i=vec3(.8);switch(p){case 1:a=Q(l,f);i=mix(vec3(.93,.8,.64),mix(vec3(.69+texture(tN,l.xz*1e-4).x,.67,.65),vec3(.38,.52,.23),dot(a,vec3(0,1,0))),b(l.y*.5-1.))+texture(tN,l.xz*.15).x*.1+texture(tN,l.xz*.01).y*.1;break;case 2:a=P(l);switch(q){case 1:i=vec3(1);break;case 2:i=vec3(1,0,0);break;case 3:i=vec3(.5,0,0);break;case 4:i=vec3(.2,.1,.2);break;case 5:i=vec3(1,.95,.8);break;case-1:i=vec3(.64000005,.48000002,.32000002);break;default:vec4 b=texture(tN,l.xy*.35)*a.z+texture(tN,l.yz*.35)*a.x+texture(tN,l.xz*.35)*a.y-.5;i+=.125*(b.x-b.y+b.z-b.w);break;}}j=normalize(mix(a,j,o));}}float r=m||p==2&&q>0?pow(b(dot(iS.xyz,reflect(c,j))),50.):0.,s=b(dot(iS.xyz,j)),t=b(dot(iS.xyz*vec3(-1,1,-1),j)),u=s+t*.15;if(p==1)u=pow(u*mix(.9,1.02,s*s),1.+s*.6);u=mix(u,s,o);float v=1.;if(N!=0)v=S(a+c*k,k,j,1.);if((iF&1)!=0&&f<20.){float a=pow(clamp(dot(iD.xyz,c),0.,1.),32.)*smoothstep(10.,0.,f);u+=a*max(dot(j,-c),0.)*(1.-u);v+=a*(1.-v);}i=mix(i,n,o);i=(i*vec3(1.065,.95,.85)*u+r)*mix(.38+(1.-u)*.2,1.,v);return V(i,k,c);}void main_c(){vec3 a=vec3(0,0,1);a.xz*=s(FC.x*.049087387+3.1415927);oC=d(.2-O(vec3(iP.x,iP.y+(FC.y/64.-1.)-.8,iP.z)+normalize(a)*.15));}void main_p(){vec2 b=FC/128.-1.+.001953125;vec3 c=normalize(iM*vec3(b.x*-1.5,b.y,2.4142134));float d=R(iP,c,.0046875,.15);uint e=floatBitsToUint(d>=150.?150.:d-a);oC=vec4(float(e>>24&255u)/255.,float(e>>16&255u)/255.,float(e>>8&255u)/255.,float(e&255u)/255.);}void main_m(){vec2 a=FC/(iR*.5)-1.;vec3 b=normalize(iM*vec3(a.x*-1.5,a.y,2.4142134));oC=vec4(W(iP,b),1);}float X(vec2 a,float c,float d,float e,float f){vec2 g=a-vec2(c,d);return b(1.-smoothstep(e-e*f,e,dot(g,g)*4.));}void main_h(){vec2 a=FC/(iR*.5)-1.,e=vec2(1.3,1),g=vec2(0);float h=0.,i=1.,j=1.,k=0.,l=1.;while(l<11.){vec3 b=f(21.1+a*e*s(l++*2.4));g+=b.yz;h+=j*(1.-b.x)/(1.+dot(g,g));i+=j*(.5-b.x);k+=j;j*=.5;e*=1.8;}h/=k;i*=.5;float m=length(a*(1.2-i+h))*b(i+.55-.5*h*a.x*(1.-a.y*.5)),n=X(a,-.45,-.52,1.,2.3)+X(a,-.6,-.1,1.,3.3)+X(a,.6,-.7,1.,5.)+X(a,.84,.84,h,i*5.);m=b(1.-c(m,1.-mix(0.,h*2.,n),.05+i*.5));vec2 o=1.-abs(a)+h*.04;m=c(m,c(o.x,o.y,.3)*2.,.1);oC=d(c(h,m,.01)*1.33-.045);}";

export const makeTestShader = (shader: TestShader, options?: SpglslAngleCompileOptions) => {
  const testShader = async () => {
    const compiled = await spglslAngleCompile({
      compileMode: "Optimize",
      language: shader.shaderType,
      minify: true,
      beautify: true,
      mangle: false,
      ...options,
      mainFilePath: shader.name,
      mainSourceCode: shader.sourceCode,
    });

    const info = `${compiled.compileMode} ${compiled.language} ${compiled.outputVersion} ./${
      shader.name
    }\n${compiled.infoLog.toString({
      colors: true,
    })}`;

    // if (shader.name.includes("island")) {
    console.log(compiled.output);
    // }

    expect(compiled.valid).to.equal(true, info);
    expect(compiled.infoLog.getCounts().errors).to.equal(0, info);

    /* console.log(chalk.magentaBright(`----- ${shader.name} -----`));
    console.log(shader.sourceCode);
    console.log(chalk.magentaBright("---------"));
    console.log(chalk.greenBright(compiled.output !== null ? compiled.output : null));
    console.log(chalk.magentaBright("---------")); */

    const validated = await spglslAngleCompile({
      compileMode: "Validate",
      mainFilePath: compiled.mainFilePath,
      mainSourceCode: compiled.output || shader.sourceCode,
      customData: compiled.customData,
      language: compiled.language,
    });

    const info1 = `${validated.compileMode} ${validated.language} ${validated.outputVersion} ./${
      shader.name
    }\n${validated.infoLog.toString({
      colors: true,
    })} ${compiled.output}`;

    expect(validated.valid).to.equal(true, info1);
    expect(validated.infoLog.getCounts().errors).to.equal(0, info);

    const out = compiled.output || "";
    const outZlibSize = zlib.gzipSync(out, { level: 9 }).length;

    console.log("output:", chalk.cyanBright(`${out.length} bytes (gzipped ${outZlibSize} bytes)`));

    if (shader.name.includes("island")) {
      console.log(
        "refern:",
        `${fragmentShaderCode.length} bytes (gzipped ${zlib.gzipSync(fragmentShaderCode, { level: 9 }).length} bytes)`,
      );
    }
    // console.log(fragmentShaderCode)
    return compiled;
  };

  Reflect.defineProperty(testShader, "name", {
    value: `./${testShader.name}`,
    configurable: true,
    enumerable: false,
    writable: true,
  });

  return testShader;
};
