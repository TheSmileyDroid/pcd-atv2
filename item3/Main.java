public class Main {

	private static void nap(long l) {
		try {
			Thread.sleep(l);
		} catch (InterruptedException e) {
		}
	}

	public static void main(String[] a) {
		final CarWindow win = new CarWindow();

		win.pack();
		win.setVisible(true);

		new Thread(new Runnable() {
			public void run() {
				long lastUpdateTime = System.currentTimeMillis();
				long targetFrameTime = 1000 / 30; // 30 FPS
				while (true) {
					nap(targetFrameTime - (System.currentTimeMillis() - lastUpdateTime));
					lastUpdateTime = System.currentTimeMillis();
					win.repaint();
				}
			}
		}).start();

	}

}
