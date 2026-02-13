<!doctype html>
<html lang="es">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover" />
  <title>💘 14 de Febrero</title>
  <style>
    :root{
      --bg:#05060a;
      --pink:#ff4fd8;
      --pink2:#ff8fe9;
      --text:#e9f0ff;
    }
    html,body{height:100%; margin:0; background:var(--bg); overflow:hidden; font-family: system-ui, -apple-system, Segoe UI, Roboto, Arial;}
    #wrap{position:fixed; inset:0; display:block;}
    canvas{position:absolute; inset:0; width:100%; height:100%;}
    .ui{
      position:fixed; left:0; right:0; top:0;
      display:flex; justify-content:space-between; align-items:center;
      padding:12px 14px; gap:10px;
      pointer-events:none;
      color:rgba(233,240,255,.9);
      text-shadow:0 2px 10px rgba(0,0,0,.6);
      font-weight:600;
    }
    .pill{
      pointer-events:auto;
      backdrop-filter: blur(10px);
      background: rgba(10,12,20,.45);
      border: 1px solid rgba(255,255,255,.10);
      padding:10px 12px;
      border-radius:999px;
      display:flex; align-items:center; gap:10px;
      box-shadow: 0 10px 25px rgba(0,0,0,.25);
    }
    .pill input{
      width: 150px; max-width: 40vw;
      border:none; outline:none;
      background:transparent;
      color: var(--text);
      font-weight:600;
    }
    .pill button{
      border:none; outline:none; cursor:pointer;
      color: var(--text);
      background: rgba(255,79,216,.18);
      border:1px solid rgba(255,79,216,.35);
      padding:8px 10px;
      border-radius:999px;
      font-weight:700;
    }
    .hint{
      pointer-events:none;
      opacity:.75;
      font-weight:600;
      font-size:12px;
      line-height:1.2;
      text-align:right;
    }
    .hint b{opacity:1}
    @media (max-width:520px){
      .hint{display:none;}
      .pill input{width: 120px;}
    }
  </style>
</head>
<body>
  <div id="wrap">
    <canvas id="c"></canvas>
  </div>

  <div class="ui">
    <div class="pill">
      <span>Para:</span>
      <input id="name" value="Mi Amor" maxlength="24" />
      <button id="go">▶</button>
    </div>
    <div class="hint">
      Toca/clic para <b>reiniciar</b><br/>y ver la animación otra vez
    </div>
  </div>

<script>
(() => {
  const canvas = document.getElementById('c');
  const ctx = canvas.getContext('2d', { alpha: true });

  // ====== Config editable ======
  const CFG = {
    matrixSpeed: 1.25,
    matrixFont: 16,
    matrixFade: 0.08,
    matrixColor: 'rgba(255,79,216,0.85)',
    glowColor: 'rgba(255,79,216,0.55)',
    particleColor: 'rgba(255,255,255,0.95)',
    particleCount: 1700,            // baja a 1200 si tu cel va lento
    settleSpeed: 0.075,
    disperseSpeed: 0.020,
    heartColor: 'rgba(255,79,216,0.95)',
  };

  // ====== Resize ======
  let W=0, H=0, DPR=1;
  function resize(){
    DPR = Math.min(2, window.devicePixelRatio || 1);
    W = Math.floor(innerWidth);
    H = Math.floor(innerHeight);
    canvas.width = Math.floor(W * DPR);
    canvas.height = Math.floor(H * DPR);
    canvas.style.width = W+'px';
    canvas.style.height = H+'px';
    ctx.setTransform(DPR,0,0,DPR,0,0);
    matrixInit();
  }
  addEventListener('resize', resize, {passive:true});

  // ====== Matrix rain ======
  const chars = ('01アイウエオカキクケコサシスセソタチツテトナニヌネノ' +
                 'ハヒフヘホマミムメモヤユヨラリルレロワヲン' +
                 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ' +
                 '0123456789@#$%&*+-=?<>').split('');
  let columns=0, drops=[];
  function matrixInit(){
    const fs = CFG.matrixFont;
    columns = Math.ceil(W / fs);
    drops = new Array(columns).fill(0).map(()=>Math.random()*H);
  }

  function drawMatrix(){
    // fade
    ctx.fillStyle = `rgba(5,6,10,${CFG.matrixFade})`;
    ctx.fillRect(0,0,W,H);

    ctx.font = `${CFG.matrixFont}px monospace`;
    for(let i=0;i<columns;i++){
      const x = i*CFG.matrixFont;
      const y = drops[i];

      const ch = chars[(Math.random()*chars.length)|0];
      ctx.fillStyle = CFG.matrixColor;
      ctx.shadowColor = CFG.glowColor;
      ctx.shadowBlur = 12;
      ctx.fillText(ch, x, y);

      // advance
      drops[i] += CFG.matrixFont * CFG.matrixSpeed;
      if(drops[i] > H + Math.random()*400) drops[i] = -Math.random()*300;
    }
    ctx.shadowBlur = 0;
  }

  // ====== Targets (texto en canvas oculto -> puntos) ======
  const off = document.createElement('canvas');
  const ox = off.getContext('2d');
  function makeTarget(drawFn){
    off.width = Math.floor(W);
    off.height = Math.floor(H);
    ox.clearRect(0,0,W,H);
    drawFn(ox);
    const img = ox.getImageData(0,0,W,H).data;

    const pts = [];
    // muestreo inteligente: menos puntos en pantallas chicas
    const step = Math.max(3, Math.round(Math.min(W,H)/260));
    for(let y=0;y<H;y+=step){
      for(let x=0;x<W;x+=step){
        const a = img[(y*W + x)*4 + 3];
        if(a > 10) pts.push({x, y});
      }
    }
    return pts;
  }

  function drawCenteredText(ox, text, size, y, color='white', weight=800){
    ox.save();
    ox.textAlign='center';
    ox.textBaseline='middle';
    ox.fillStyle=color;
    ox.font=`${weight} ${size}px system-ui, -apple-system, Segoe UI, Roboto, Arial`;
    ox.shadowColor='rgba(255,79,216,.35)';
    ox.shadowBlur=18;
    ox.fillText(text, W/2, y);
    ox.restore();
  }

  function drawHeart(ox, cx, cy, s){
    ox.save();
    ox.translate(cx, cy);
    ox.scale(s, s);
    ox.beginPath();
    // corazón paramétrico simple
    for(let t=0;t<=Math.PI*2+0.02;t+=0.02){
      const x = 16*Math.pow(Math.sin(t),3);
      const y = -(13*Math.cos(t) - 5*Math.cos(2*t) - 2*Math.cos(3*t) - Math.cos(4*t));
      if(t===0) ox.moveTo(x,y); else ox.lineTo(x,y);
    }
    ox.closePath();
    ox.lineWidth = 1.8;
    ox.strokeStyle = 'rgba(255,79,216,.95)';
    ox.shadowColor='rgba(255,79,216,.55)';
    ox.shadowBlur=18;
    ox.stroke();
    ox.restore();
  }

  // ====== Particles ======
  let particles=[];
  function initParticles(n){
    particles = new Array(n).fill(0).map(()=>({
      x: Math.random()*W,
      y: Math.random()*H,
      vx:(Math.random()-0.5)*2,
      vy:(Math.random()-0.5)*2,
      tx: Math.random()*W,
      ty: Math.random()*H,
    }));
  }

  function setTargets(points){
    // si hay menos puntos que partículas, recicla
    for(let i=0;i<particles.length;i++){
      const p = particles[i];
      const t = points[i % points.length];
      p.tx = t.x; p.ty = t.y;
    }
  }

  function stepParticles(k){
    // k: 0..1 (más alto = más rápido hacia target)
    const s = CFG.settleSpeed * k;
    for(const p of particles){
      const dx = p.tx - p.x;
      const dy = p.ty - p.y;
      p.vx = p.vx*0.86 + dx*s;
      p.vy = p.vy*0.86 + dy*s;
      p.x += p.vx;
      p.y += p.vy;
    }
  }

  function disperse(){
    for(const p of particles){
      p.vx += (Math.random()-0.5)*8;
      p.vy += (Math.random()-0.5)*8;
    }
  }

  function drawParticles(alpha=1){
    ctx.save();
    ctx.globalAlpha = alpha;
    ctx.fillStyle = CFG.particleColor;
    ctx.shadowColor = 'rgba(255,255,255,.25)';
    ctx.shadowBlur = 10;

    // puntos “tipo LED”
    for(const p of particles){
      ctx.beginPath();
      ctx.arc(p.x, p.y, 1.35, 0, Math.PI*2);
      ctx.fill();
    }
    ctx.restore();
  }

  // ====== Timeline / escenas ======
  let t0=0;
  let running=true;
  let targets = [];
  let scene = 0;

  function buildScenes(name){
    const big = Math.min(W,H) * 0.42;
    const yMid = H*0.45;

    const s1 = makeTarget((ox)=>{
      drawCenteredText(ox, '1', Math.floor(big), yMid, 'white', 900);
    });

    const s2 = makeTarget((ox)=>{
      const line1 = 'You';
      const line2 = 'Are';
      const line3 = `My Love`;
      const size = Math.floor(Math.min(W,H) * 0.18);
      drawCenteredText(ox, line1, size, H*0.35, 'white', 900);
      drawCenteredText(ox, line2, size, H*0.50, 'white', 900);
      drawCenteredText(ox, line3, size, H*0.65, 'white', 900);
    });

    const s3 = makeTarget((ox)=>{
      // Corazón grande al centro
      const scale = Math.min(W,H) * 0.015;
      drawHeart(ox, W/2, H*0.48, scale);

      // texto dentro
      const inside = Math.floor(Math.min(W,H)*0.055);
      drawCenteredText(ox, `I Love You`, inside, H*0.46, 'rgba(255,143,233,.98)', 800);
      drawCenteredText(ox, name, Math.floor(inside*0.92), H*0.52, 'rgba(233,240,255,.95)', 700);
    });

    targets = [s1, s2, s3];
  }

  function restart(){
    const name = (document.getElementById('name').value || 'Mi Amor').trim().slice(0,24);
    buildScenes(name);

    // Ajuste automático de rendimiento
    const base = CFG.particleCount;
    const scale = Math.min(1, (W*H) / (900*1600));
    const n = Math.floor(base * (0.75 + 0.55*scale));
    initParticles(n);

    scene = 0;
    setTargets(targets[scene]);
    disperse();
    t0 = performance.now();
    running = true;
  }

  // ====== Render loop ======
  function loop(now){
    if(!running) return;

    drawMatrix();

    const elapsed = (now - t0) / 1000;

    // transiciones:
    // 0) formar "1"  (0.0 - 2.5s)
    // 1) disolver + formar "You Are My Love" (2.5 - 7.5s)
    // 2) disolver + formar corazón (7.5 - 12.5s)
    // 3) mantener (12.5+)
    if(elapsed < 2.5){
      if(scene !== 0){
        scene = 0; setTargets(targets[0]); disperse();
      }
      stepParticles(1.0);
      drawParticles(0.95);
    } else if(elapsed < 7.5){
      if(scene !== 1){
        scene = 1; setTargets(targets[1]); disperse();
      }
      stepParticles(1.0);
      drawParticles(0.95);
    } else if(elapsed < 12.5){
      if(scene !== 2){
        scene = 2; setTargets(targets[2]); disperse();
      }
      stepParticles(1.0);
      drawParticles(0.98);
    } else {
      // mantener y hacer micro-brillo
      stepParticles(0.55);
      const pulse = 0.90 + 0.08*Math.sin(elapsed*2.2);
      drawParticles(pulse);
    }

    requestAnimationFrame(loop);
  }

  // ====== Interacciones ======
  const goBtn = document.getElementById('go');
  goBtn.addEventListener('click', restart);

  // toque/clic en cualquier parte reinicia (cómodo para mostrarlo)
  addEventListener('pointerdown', (e)=>{
    // evita reiniciar si tocan el input/botón
    const tag = (e.target && e.target.tagName) ? e.target.tagName.toLowerCase() : '';
    if(tag === 'input' || tag === 'button') return;
    restart();
  }, {passive:true});

  // init
  resize();
  restart();
  requestAnimationFrame(loop);
})();
</script>
</body>
</html><!doctype html>
<html lang="es">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover" />
  <title>💘 14 de Febrero</title>
  <style>
    :root{
      --bg:#05060a;
      --pink:#ff4fd8;
      --pink2:#ff8fe9;
      --text:#e9f0ff;
    }
    html,body{height:100%; margin:0; background:var(--bg); overflow:hidden; font-family: system-ui, -apple-system, Segoe UI, Roboto, Arial;}
    #wrap{position:fixed; inset:0; display:block;}
    canvas{position:absolute; inset:0; width:100%; height:100%;}
    .ui{
      position:fixed; left:0; right:0; top:0;
      display:flex; justify-content:space-between; align-items:center;
      padding:12px 14px; gap:10px;
      pointer-events:none;
      color:rgba(233,240,255,.9);
      text-shadow:0 2px 10px rgba(0,0,0,.6);
      font-weight:600;
    }
    .pill{
      pointer-events:auto;
      backdrop-filter: blur(10px);
      background: rgba(10,12,20,.45);
      border: 1px solid rgba(255,255,255,.10);
      padding:10px 12px;
      border-radius:999px;
      display:flex; align-items:center; gap:10px;
      box-shadow: 0 10px 25px rgba(0,0,0,.25);
    }
    .pill input{
      width: 150px; max-width: 40vw;
      border:none; outline:none;
      background:transparent;
      color: var(--text);
      font-weight:600;
    }
    .pill button{
      border:none; outline:none; cursor:pointer;
      color: var(--text);
      background: rgba(255,79,216,.18);
      border:1px solid rgba(255,79,216,.35);
      padding:8px 10px;
      border-radius:999px;
      font-weight:700;
    }
    .hint{
      pointer-events:none;
      opacity:.75;
      font-weight:600;
      font-size:12px;
      line-height:1.2;
      text-align:right;
    }
    .hint b{opacity:1}
    @media (max-width:520px){
      .hint{display:none;}
      .pill input{width: 120px;}
    }
  </style>
</head>
<body>
  <div id="wrap">
    <canvas id="c"></canvas>
  </div>

  <div class="ui">
    <div class="pill">
      <span>Para:</span>
      <input id="name" value="Mi Amor" maxlength="24" />
      <button id="go">▶</button>
    </div>
    <div class="hint">
      Toca/clic para <b>reiniciar</b><br/>y ver la animación otra vez
    </div>
  </div>

<script>
(() => {
  const canvas = document.getElementById('c');
  const ctx = canvas.getContext('2d', { alpha: true });

  // ====== Config editable ======
  const CFG = {
    matrixSpeed: 1.25,
    matrixFont: 16,
    matrixFade: 0.08,
    matrixColor: 'rgba(255,79,216,0.85)',
    glowColor: 'rgba(255,79,216,0.55)',
    particleColor: 'rgba(255,255,255,0.95)',
    particleCount: 1700,            // baja a 1200 si tu cel va lento
    settleSpeed: 0.075,
    disperseSpeed: 0.020,
    heartColor: 'rgba(255,79,216,0.95)',
  };

  // ====== Resize ======
  let W=0, H=0, DPR=1;
  function resize(){
    DPR = Math.min(2, window.devicePixelRatio || 1);
    W = Math.floor(innerWidth);
    H = Math.floor(innerHeight);
    canvas.width = Math.floor(W * DPR);
    canvas.height = Math.floor(H * DPR);
    canvas.style.width = W+'px';
    canvas.style.height = H+'px';
    ctx.setTransform(DPR,0,0,DPR,0,0);
    matrixInit();
  }
  addEventListener('resize', resize, {passive:true});

  // ====== Matrix rain ======
  const chars = ('01アイウエオカキクケコサシスセソタチツテトナニヌネノ' +
                 'ハヒフヘホマミムメモヤユヨラリルレロワヲン' +
                 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ' +
                 '0123456789@#$%&*+-=?<>').split('');
  let columns=0, drops=[];
  function matrixInit(){
    const fs = CFG.matrixFont;
    columns = Math.ceil(W / fs);
    drops = new Array(columns).fill(0).map(()=>Math.random()*H);
  }

  function drawMatrix(){
    // fade
    ctx.fillStyle = `rgba(5,6,10,${CFG.matrixFade})`;
    ctx.fillRect(0,0,W,H);

    ctx.font = `${CFG.matrixFont}px monospace`;
    for(let i=0;i<columns;i++){
      const x = i*CFG.matrixFont;
      const y = drops[i];

      const ch = chars[(Math.random()*chars.length)|0];
      ctx.fillStyle = CFG.matrixColor;
      ctx.shadowColor = CFG.glowColor;
      ctx.shadowBlur = 12;
      ctx.fillText(ch, x, y);

      // advance
      drops[i] += CFG.matrixFont * CFG.matrixSpeed;
      if(drops[i] > H + Math.random()*400) drops[i] = -Math.random()*300;
    }
    ctx.shadowBlur = 0;
  }

  // ====== Targets (texto en canvas oculto -> puntos) ======
  const off = document.createElement('canvas');
  const ox = off.getContext('2d');
  function makeTarget(drawFn){
    off.width = Math.floor(W);
    off.height = Math.floor(H);
    ox.clearRect(0,0,W,H);
    drawFn(ox);
    const img = ox.getImageData(0,0,W,H).data;

    const pts = [];
    // muestreo inteligente: menos puntos en pantallas chicas
    const step = Math.max(3, Math.round(Math.min(W,H)/260));
    for(let y=0;y<H;y+=step){
      for(let x=0;x<W;x+=step){
        const a = img[(y*W + x)*4 + 3];
        if(a > 10) pts.push({x, y});
      }
    }
    return pts;
  }

  function drawCenteredText(ox, text, size, y, color='white', weight=800){
    ox.save();
    ox.textAlign='center';
    ox.textBaseline='middle';
    ox.fillStyle=color;
    ox.font=`${weight} ${size}px system-ui, -apple-system, Segoe UI, Roboto, Arial`;
    ox.shadowColor='rgba(255,79,216,.35)';
    ox.shadowBlur=18;
    ox.fillText(text, W/2, y);
    ox.restore();
  }

  function drawHeart(ox, cx, cy, s){
    ox.save();
    ox.translate(cx, cy);
    ox.scale(s, s);
    ox.beginPath();
    // corazón paramétrico simple
    for(let t=0;t<=Math.PI*2+0.02;t+=0.02){
      const x = 16*Math.pow(Math.sin(t),3);
      const y = -(13*Math.cos(t) - 5*Math.cos(2*t) - 2*Math.cos(3*t) - Math.cos(4*t));
      if(t===0) ox.moveTo(x,y); else ox.lineTo(x,y);
    }
    ox.closePath();
    ox.lineWidth = 1.8;
    ox.strokeStyle = 'rgba(255,79,216,.95)';
    ox.shadowColor='rgba(255,79,216,.55)';
    ox.shadowBlur=18;
    ox.stroke();
    ox.restore();
  }

  // ====== Particles ======
  let particles=[];
  function initParticles(n){
    particles = new Array(n).fill(0).map(()=>({
      x: Math.random()*W,
      y: Math.random()*H,
      vx:(Math.random()-0.5)*2,
      vy:(Math.random()-0.5)*2,
      tx: Math.random()*W,
      ty: Math.random()*H,
    }));
  }

  function setTargets(points){
    // si hay menos puntos que partículas, recicla
    for(let i=0;i<particles.length;i++){
      const p = particles[i];
      const t = points[i % points.length];
      p.tx = t.x; p.ty = t.y;
    }
  }

  function stepParticles(k){
    // k: 0..1 (más alto = más rápido hacia target)
    const s = CFG.settleSpeed * k;
    for(const p of particles){
      const dx = p.tx - p.x;
      const dy = p.ty - p.y;
      p.vx = p.vx*0.86 + dx*s;
      p.vy = p.vy*0.86 + dy*s;
      p.x += p.vx;
      p.y += p.vy;
    }
  }

  function disperse(){
    for(const p of particles){
      p.vx += (Math.random()-0.5)*8;
      p.vy += (Math.random()-0.5)*8;
    }
  }

  function drawParticles(alpha=1){
    ctx.save();
    ctx.globalAlpha = alpha;
    ctx.fillStyle = CFG.particleColor;
    ctx.shadowColor = 'rgba(255,255,255,.25)';
    ctx.shadowBlur = 10;

    // puntos “tipo LED”
    for(const p of particles){
      ctx.beginPath();
      ctx.arc(p.x, p.y, 1.35, 0, Math.PI*2);
      ctx.fill();
    }
    ctx.restore();
  }

  // ====== Timeline / escenas ======
  let t0=0;
  let running=true;
  let targets = [];
  let scene = 0;

  function buildScenes(name){
    const big = Math.min(W,H) * 0.42;
    const yMid = H*0.45;

    const s1 = makeTarget((ox)=>{
      drawCenteredText(ox, '1', Math.floor(big), yMid, 'white', 900);
    });

    const s2 = makeTarget((ox)=>{
      const line1 = 'You';
      const line2 = 'Are';
      const line3 = `My Love`;
      const size = Math.floor(Math.min(W,H) * 0.18);
      drawCenteredText(ox, line1, size, H*0.35, 'white', 900);
      drawCenteredText(ox, line2, size, H*0.50, 'white', 900);
      drawCenteredText(ox, line3, size, H*0.65, 'white', 900);
    });

    const s3 = makeTarget((ox)=>{
      // Corazón grande al centro
      const scale = Math.min(W,H) * 0.015;
      drawHeart(ox, W/2, H*0.48, scale);

      // texto dentro
      const inside = Math.floor(Math.min(W,H)*0.055);
      drawCenteredText(ox, `I Love You`, inside, H*0.46, 'rgba(255,143,233,.98)', 800);
      drawCenteredText(ox, name, Math.floor(inside*0.92), H*0.52, 'rgba(233,240,255,.95)', 700);
    });

    targets = [s1, s2, s3];
  }

  function restart(){
    const name = (document.getElementById('name').value || 'Mi Amor').trim().slice(0,24);
    buildScenes(name);

    // Ajuste automático de rendimiento
    const base = CFG.particleCount;
    const scale = Math.min(1, (W*H) / (900*1600));
    const n = Math.floor(base * (0.75 + 0.55*scale));
    initParticles(n);

    scene = 0;
    setTargets(targets[scene]);
    disperse();
    t0 = performance.now();
    running = true;
  }

  // ====== Render loop ======
  function loop(now){
    if(!running) return;

    drawMatrix();

    const elapsed = (now - t0) / 1000;

    // transiciones:
    // 0) formar "1"  (0.0 - 2.5s)
    // 1) disolver + formar "You Are My Love" (2.5 - 7.5s)
    // 2) disolver + formar corazón (7.5 - 12.5s)
    // 3) mantener (12.5+)
    if(elapsed < 2.5){
      if(scene !== 0){
        scene = 0; setTargets(targets[0]); disperse();
      }
      stepParticles(1.0);
      drawParticles(0.95);
    } else if(elapsed < 7.5){
      if(scene !== 1){
        scene = 1; setTargets(targets[1]); disperse();
      }
      stepParticles(1.0);
      drawParticles(0.95);
    } else if(elapsed < 12.5){
      if(scene !== 2){
        scene = 2; setTargets(targets[2]); disperse();
      }
      stepParticles(1.0);
      drawParticles(0.98);
    } else {
      // mantener y hacer micro-brillo
      stepParticles(0.55);
      const pulse = 0.90 + 0.08*Math.sin(elapsed*2.2);
      drawParticles(pulse);
    }

    requestAnimationFrame(loop);
  }

  // ====== Interacciones ======
  const goBtn = document.getElementById('go');
  goBtn.addEventListener('click', restart);

  // toque/clic en cualquier parte reinicia (cómodo para mostrarlo)
  addEventListener('pointerdown', (e)=>{
    // evita reiniciar si tocan el input/botón
    const tag = (e.target && e.target.tagName) ? e.target.tagName.toLowerCase() : '';
    if(tag === 'input' || tag === 'button') return;
    restart();
  }, {passive:true});

  // init
  resize();
  restart();
  requestAnimationFrame(loop);
})();
</script>
</body>
</html>