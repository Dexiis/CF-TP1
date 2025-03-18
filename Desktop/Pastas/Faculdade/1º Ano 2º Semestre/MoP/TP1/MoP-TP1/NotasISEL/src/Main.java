import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        boolean mainLoop = true;
        while (mainLoop) { //loop principal do jogo
            System.out.println("Quantas notas pretende inserir?");
            int gradesNumber = input.nextInt();
            double[] notas = new double[gradesNumber];
            int index = 0;
            while (true) { //loop do pedido das notas
                try {
                    while (index < gradesNumber) {
                        System.out.print((index + 1) + "ª nota: ");
                        notas[index] = input.nextDouble();
                        if (notas[index] < 0) System.out.println("O valor introduzido está incorreto."); //não aceita número negativos
                        else index++;
                    }
                    break;
                } catch (Exception e) { //não aceita valores que possam provocar um erro no programa
                    System.out.println("O valor introduzido está incorreto.");
                    input.next();
                }
            }
            notas = sort(notas, notas.length);
            double averageGrade = averageGrade(notas, gradesNumber);
            System.out.println();
            System.out.println("Nota mínima: " + notas[0]);
            System.out.println("Nota máxima: " + notas[gradesNumber - 1]);
            System.out.println("Nota média: " + averageGrade);
            System.out.println("Notas abaixo da média: " + aboveBelowGrade(notas, gradesNumber, averageGrade)[0]);
            System.out.println("Notas iguais à média: " + aboveBelowGrade(notas, gradesNumber, averageGrade)[1]);
            System.out.println("Notas acima da média: " + aboveBelowGrade(notas, gradesNumber, averageGrade)[2]);
            System.out.println();
            System.out.println("Pretende repetir? ('n' para terminar)");
            String programStatus = input.next();
            System.out.println(); System.out.println(); System.out.println();
            if (programStatus.equals("n")) mainLoop = false;
        }
    }

    private static double[] sort(double[] notas, int length) { //bubblesort feito com recursividade
        double tempValue;
        for (int i = 0; i < length - 1; i++) {
            if (notas[i] > notas[i + 1]) {
                tempValue = notas[i];
                notas[i] = notas[i + 1];
                notas[i + 1] = tempValue;
            }
        }
        if (length >= 0) return sort(notas, length - 1);
        else return notas;
    }

    private static double averageGrade(double[] notas, int length) {
        double sum = 0;
        for (int i = 0; i < length; i++) sum += notas[i];
        return (sum / length);
    }

    private static int[] aboveBelowGrade(double[] notas, int length, double averageGrade) {
        int[] aboveBelowAverageCount = new int[3]; //armazena cada counter em um local da variável
        for (int i = 0; i < length; i++) {
            if (notas[i] < averageGrade) aboveBelowAverageCount[0] += 1;
            else if (notas[i] == averageGrade) aboveBelowAverageCount[1] += 1;
            else if (notas[i] > averageGrade) aboveBelowAverageCount[2] += 1;
        }
        return aboveBelowAverageCount;
    }
}

